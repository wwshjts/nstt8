.PHONY : all clean 
# setup dirs
src_dir := src
build_dir := build
test_dir := tests

# target setup
target_name := 
object_files := io_lib 

# CC setup
CC := g++
flags := -fsanitize=address -fsanitize=undefined -fsanitize=leak

# setup 
target_src := $(addprefix $(src_dir)/, $(target_name)) 
target_src := $(addsuffix .cpp, $(target_src))

target := $(addprefix $(build_dir)/, $(target_name)) 
target := $(addsuffix .o, $(target))

objects := $(addprefix $(build_dir)/, $(object_files))
objects := $(addsuffix .o, $(objects))

headers := $(addprefix $(src_dir)/, $(object_files)) 
headers := $(addsuffix .h, $(headers)) 

# build
all : $(build_dir) $(target) $(objects) 

$(build_dir) : 
	@mkdir $(build_dir)

$(target) : $(objects) $(target_src)
	$(CC) $(target_src) $(objects) -o $(target) $(flags)

$(objects) : $(build_dir)/%.o : $(src_dir)/%.cpp $(src_dir)/%.h  
	$(CC) -c $(src_dir)/$*.cpp -o $(build_dir)/$*.o $(flags) 



#test setup
test_files_src := $(wildcard $(test_dir)/*.cpp)
# TODO
test_files := $(test_files_src:.cpp=.out)


test : $(test_files)
	@echo "All test passed"

$(test_files) : %.out: $(target) $(headers) $(objects) $(test_files_src)
	$(CC) $*.cpp $(objects) -o $*.out -lgtest -lgtest_main $(flags)
	$*.out

clean :
	rm $(objects) $(target)

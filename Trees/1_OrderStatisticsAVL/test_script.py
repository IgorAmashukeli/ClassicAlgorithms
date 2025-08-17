import subprocess
import glob

expected_outputs_data_error = {
    "test1.txt": "\n",
    "test2.txt": "\n",
    "test3.txt": "Wrong index for k-th order statistic. Error. \n",
    "test4.txt": "Wrong index for k-th order statistic. Error. \n",
    "test5.txt": "NO number followed. Error.\n",
    "test6.txt": "You entered a duplicate. Error. \n",
    "test7.txt": "Wrong index for k-th order statistic. Error. \n",
    "test8.txt": "Value out of long long range. Error.\n"
}

compile_cmd = ["clang++", "trial_task.cpp", "-o", "trial_task", "-std=c++20", "-fsanitize=address"]
try:
    subprocess.check_call(compile_cmd)
    print("Compilation for tests with errors in data input is successful\n")
except subprocess.CalledProcessError as e:
    print(f"Compilation failed: {e}")
    exit(1)

test_files = sorted(glob.glob("test*.txt"))
print("Running on 8 tests with errors in data input to check, how console application work")

if not test_files:
    print("No test files found.")
    exit(1)

for (i, test_file) in enumerate(test_files):
    with open(test_file, 'r') as f:
        input_data = f.read()
    
    try:
        result = subprocess.run(["./trial_task"], input=input_data, text=True, capture_output=True)
        output = result.stdout
        if result.stderr:
            print("Errors:")
            print(result.stderr)
        
        expected = expected_outputs_data_error.get(test_file, "")
        if output == expected:
            print("Test " +  str(i + 1) +  " passed")
        else:
            print("Test failed!")
            print(f"Expected:\n{expected}\n")
    except Exception as e:
        print(f"Run failed: {e}\n")

print("Tests with corrupted data passed\n")

print("Running stress tests to check how SetAVL class works")

compile_cmd2 = ["clang++", "class_tests.cpp", "-o", "class_tests", "-std=c++20", "-fsanitize=address"]
try:
    subprocess.check_call(compile_cmd2)
    print("Compilation for main tests successful\n")
except subprocess.CalledProcessError as e:
    print(f"Compilation failed: {e}")
    exit(1)

result = subprocess.run(["./class_tests"], input=input_data, text=True, capture_output=True)
output = result.stdout
if result.stderr:
    print("Errors:")
    print(result.stderr)
else:
    print(output)
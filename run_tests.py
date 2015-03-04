import subprocess

class bcolors:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'

class Test:
    def __init__(self, code, expected_result, comment):
        self.code = code
        self.expected_result = expected_result
        self.comment = comment

    def execute(self, application):
        cmd = 'printf "{}" | {}'.format(test.code, application)

        proc = subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE)
        result = proc.communicate()[0].split("\n")[-2].strip() # Only check last returned line
        return result

def prepare_test(lines):
    expected_result, lines = extract_expected_result(lines)
    comment, lines = extract_comments(lines)
    code = extract_code(lines)
    return Test(code, expected_result, comment)

def run_test(test):
    print(test.comment)

    result = test.execute("bin/alisp")
    cmd = 'printf "{}" | bin/alisp'.format(test.code)
    
    if test.expected_result == result:
        print(bcolors.OKGREEN + "Pass" + bcolors.ENDC)
    else:
        print(bcolors.FAIL + "Fail\nExpected: {}\nGot: {}".format(test.expected_result, result) + bcolors.ENDC)
    print("")
    

def extract_expected_result(lines):
    expected_result = [line.upper() for line in lines if line.startswith(";=>")][0].split(";=>")[1]
    other_lines = [line for line in lines if not line.startswith(";=>")]
    return expected_result, other_lines

def extract_comments(lines):
    comments = [line for line in lines if line.startswith(";;")][0].split(";;")[1].lstrip()
    other_lines = [line for line in lines if not line.startswith(";;")]
    return comments, other_lines

def extract_code(lines):
    return "\n".join(lines)

with open("tests.lisp") as test_file:
    lines = [l.strip() for l in test_file.readlines()]

    line_index = 0
    current_test = []
    for line in lines:
        if len(line) is 0:
            test = prepare_test(current_test)
            run_test(test)
            current_test = []
        else:
            current_test.append(line)

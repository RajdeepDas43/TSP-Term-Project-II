import glob
import os, re
from termcolor import colored
if not os.system("g++-8 -o greedy.out greedy.cpp"):
	#tsplibData
	arr = sorted(glob.iglob('data/*.tsp'))
	#generatedData
	arr2 = glob.iglob('generatedData/*.tsp')
	# arr2 = sorted(arr2, key = lambda x: int(x.split("/")[-1].replace("test","").replace(".tsp","")))
	arr2 = sorted(arr2, key = lambda x: int(re.search('test(.*).tsp', x).group(1)) )
	for filepath in arr:
	    print(colored(filepath + ":", 'green'))
	    filepath2 = filepath[0:-3]
	    filepath2 = filepath2 + "opt.tour"
	    if not glob.glob(filepath2):
	    	filepath2 = "-"
	    os.system("./greedy.out " + filepath +" "+ filepath2)

	for filepath in arr2:
	    print(colored(filepath + ":", 'yellow'))
	    os.system("./greedy.out " + filepath)

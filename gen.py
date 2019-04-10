#!/usr/bin/python

import os
import sys
import re

## DEBUG - TEST HELPER STARTS
exBase = 'test/tc2/'

#prints the func name, and result with the given args
def dumper(func, *args):
	print func.__name__, args, " => "
	print func(*args)
	print ''
## DEBUG - TEST HELPER ENDS

baseDir = './'
#baseDir = sys.argv[1]
progName = 'prog.exe'
#progName = sys.argv[2]

sourceFiles = []
#each file is in format (abc.cpp) => {'fileName':'abc', 'filePath':'./', 'includes': ['dom.h','pat.h']}
headerFiles = []
#each file is in format (abc.h) => {'fileName':'abc', 'filePath':'./', 'methods': ['solve','getName']}
directories = []
#all subdirectories and '.'

#returns all method names declared in a header file
def getMethods(filePath):
	f = open(baseDir + filePath)
	str = f.read()
	matches = re.findall(r'\w+(?=\s*\([^\)]*\)\s*;)', str)
	return set(matches)
#dumper(getMethods, exBase + 'addTwoInt.h')

#returns the fileName from a filePath
def reducePath(filePath):
	res = re.split(r'/', filePath)
	return res[-1]
#dumper(reducePath, 'abc/def/inc.h')

#returns all file includes (except system includes) in a source file
def getIncludes(filePath):
	f = open(baseDir + filePath)
	str = f.read()
	matches = re.findall(r'#include\s+"(.*).h"', str)
	matches = map(reducePath, matches)
	return set(matches)
#dumper(getIncludes, exBase + 'main2.cpp')

#returns all file defines in a header file
def getDefines(filePath):
	f = open(baseDir + filePath)
	str = f.read()
	matches = re.findall(r'#define\s+(\S+?)[\(\s]', str)
	return matches
#dumper(getDefines, exBase + '../tmp.h')

#splits a fileName to [fileBase, fileExt], and if there are no extension/'.' in the name, returns [fileName, 'err']
def splitExtension(fileName):
	res = re.split(r'\.(?=[^.]+$)', fileName)
	if len(res) < 2:
		res.append('err')
	return res
#dumper(splitExtension, 'abc.def.ghi')

#traverses all files in baseDir, identifies header files, source files and directories
def traverseFiles():
	# sets directories, headerFiles, sourceFiles lists
	directoryList = ['.']
	while directoryList:
		currentDirectory = directoryList.pop()
		directories.append(currentDirectory)
		folderContents = os.listdir(baseDir  + currentDirectory)
		for fileName in folderContents:
			filePath = currentDirectory + '/' + fileName
			if os.path.isdir(baseDir + filePath):
				directoryList.append(filePath)
			else:
				fileExt = splitExtension(fileName)
				if (fileExt[1] == 'cpp'):
					fileDetails = {}
					fileDetails['fileName'] = fileExt[0]
					fileDetails['filePath'] = filePath
					fileDetails['includes'] = getIncludes(filePath)
					sourceFiles.append(fileDetails)
				elif (fileExt[1] == 'h'):
					fileDetails = {}
					fileDetails['fileName'] = fileExt[0]
					fileDetails['filePath'] = filePath
					fileDetails['methods'] = getMethods(filePath)
					fileDetails['defines'] = getDefines(filePath)
					headerFiles.append(fileDetails)

#returns true if a method is either called or defined in the source file
def isHeaderUsed(sourcePath, methods, defines):
	f = open(baseDir + sourcePath)
	str = f.read()
	for method in methods:
		myRegex = method + r'\s*(.*?)\s*(;|{)'
		res = re.search(myRegex, str)
		if res:
			return True
	for define in defines:
		myRegex = method
		res = re.search(myRegex, str)
		if res:
			return True
	return False
#dumper(isHeaderUsed, exBase + 'main2.cpp',  ['addTwoInt'])
#dumper(isHeaderUsed, exBase + 'addTwoInt.cpp',  ['addTwoInt'])

#checks errors and warnings, returns false on error and true on success/warning
def checkErrorsAndWarnings():
	headerSet = set()
	for header in headerFiles:
		headerSet.add(header['fileName'])
	if (len(headerFiles) != len(headerSet)):
		print 'Error! There exists more than one header files with the same name!'
		return False
	sourceSet = set()
	for source in sourceFiles:
		sourceSet.add(source['fileName'])
	if (len(sourceFiles) != len(sourceSet)):
		print 'Error! There exists more than one source files with the same name!'
		return False
	allIncludeSets = set()
	for source in sourceFiles:
		includeSet = set(source['includes'])
		allIncludeSets |= includeSet
		errFiles = includeSet - headerSet
		if errFiles:
			print 'Error! Below files are included in', source['fileName'] + '.cpp but was not found:'
			print map(lambda x:x+'.h',errFiles)
			return False
		for header in headerFiles:
			if header['fileName'] in includeSet:
				if not isHeaderUsed(source['filePath'], header['methods'], header['defines']):
					print 'Warning! The header file', header['fileName'] + '.h was included,',
					print 'but not used in the file,', source['fileName'] + '.cpp'
			else:
				if isHeaderUsed(source['filePath'], header['methods'], header['defines']):
					print 'Error!  The header file', header['fileName'] + '.h was used,',
					print 'but not included in the file,', source['fileName'] + '.cpp'
					#return False
	diffSets = headerSet - allIncludeSets
	if diffSets:
		print "Warning! Below header files are found but not included in any source file."
		print map(lambda x:x+'.h',diffSets)
	return True

#generates the make file
def generateMakeFile():
	fullInclude = " ".join(map(lambda x:'-I\"'+x+'\"',directories))
	#print fullInclude
	makeFile = open(baseDir + "Makefile","w")
#	objects = map(lambda source: source['filePath'][:-1] + 'o', sourceFiles)
	objects = map(lambda x: x['fileName'] + '.o', sourceFiles)
	makeFile.write('INCDIR =\t' + fullInclude + '\n\n')
	makeFile.write(progName + ':\t' + " ".join(objects) + '\n')
	makeFile.write('\t\tg++ ' + " ".join(objects) + " -o " + progName + '\n')
#	makeFile.write('\t\trm ' + " ".join(objects) + '\n')
	makeFile.write('\t\t@echo "Makefile finished successfully. Created program: ' + progName + '\"\n\n')
	headerNameToPath = {}
	for header in headerFiles:
		headerNameToPath[header['fileName']] = header['filePath']
	for source in sourceFiles:
		includePaths = map(lambda x: headerNameToPath[x], source['includes'])
		makeFile.write(source['fileName'] + '.o:\t' +  source['filePath'] + " " + " ".join(includePaths) + "\n")
		makeFile.write("\t\tg++ -c $(INCDIR) " + source['filePath'] + "\n\n")

#checks system arguments, calls traverseFiles, and checkErrorsAndWarnings()
#calls generateMakeFile after validating that checkErrorsAndWarnings() resulted in no error
def main():
	global baseDir, progName
	if len(sys.argv) != 2 and len(sys.argv) != 3:
		print 'Error! Please run the program as ./generatemakefile [path for c project root folder]'
		print 'Or ./generatemakefile [path] [output_program_name]'
		print 'When used with 1 argument only, default outputted program name would be \'prog.exe\''
		return
	baseDir = sys.argv[1] + '/'
	if len(sys.argv) == 3:
		progName = sys.argv[2]
	traverseFiles();
	if checkErrorsAndWarnings():
		generateMakeFile()
		print "Makefile is generated successfully!"

# call main - start the program
main()

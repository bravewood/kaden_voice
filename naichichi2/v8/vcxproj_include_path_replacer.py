#!/usr/bin/env python
#

import os
import re


def path_filename( path ):
  p = re.compile('[\/\\\\]([^\/\\\\]*)$')
  r = p.findall( path )
  if r:
    return r[0]
  else:
    return path


def get_relative_path( inc_path, file_path, vcxproj_path_list ):
  inc_ = path_filename( inc_path )
  for path in vcxproj_path_list:
    proj_ = path_filename( path )
    if inc_ == proj_:
      print "            <<vcxproj_path>> " + path
      dir_path = os.path.dirname(file_path)
      return os.path.relpath(path, dir_path) # relative path from file_path to path


def include_replacer(file_path, vcxproj_path_list):
  dir_path = os.path.dirname( file_path )
  
  #read file
  f = open(file_path)
  strfile = f.read()
  f.close()
  strfile_new = strfile
  
  #check the include file in include path
  p1 = re.compile('.*vcxproj$')
  p2 = re.compile('Include="([^"]*)"')
  r = p2.findall(strfile)
  for include_ in r:
    if p1.match( include_ ):
      include_fullpath = os.path.join(dir_path, include_)
      if not os.path.exists(include_fullpath):
        print "            --------------------------"
        include_new = get_relative_path(include_, file_path, vcxproj_path_list)
        print "            <<file_path>> " + file_path
        print "            <<before>> " + include_
        print "            <<after >> " + include_new
        # replace wrong path
        strfile_new = strfile_new.replace( 'Include="' + include_ + '"', 'Include="' + include_new + '"' )
  
  #Fix the wrong include path 
  print "==BEFORE=="
  print strfile
  print "==AFTER=="
  print strfile_new
  #write to file
  f = open(file_path, 'w')
  f.write(strfile_new)
  f.close()

######################################
current_dir = os.getcwd()
p = re.compile('.*vcxproj$')
vcxproj_path_list = []

#First: collect all *.vcxproj paths.
for root, dirs, files in os.walk(current_dir):
  for file_ in files:
    if p.search(file_):
      proj_file_path = os.path.join(root, file_)
      vcxproj_path_list.append( proj_file_path )


print vcxproj_path_list

#Second: Fix wrong include paths.
for root, dirs, files in os.walk(current_dir):
  for file_ in files:
    if p.search(file_):
      proj_file_path = os.path.join(root, file_)
      print "------------------------------"
      print proj_file_path
      include_replacer( proj_file_path, vcxproj_path_list )





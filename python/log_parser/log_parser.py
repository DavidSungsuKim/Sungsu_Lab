
# imports
import sys
import re                                            # for regular-expression
import matplotlib.pyplot as plt                      # for plotting graphs
from datetime import date

# parameters
args = sys.argv
for arg in args:
   print(arg)

strs_to_filter_by = ["[RCP] WA_15: tgt=0.0"]         # strings in the array that we want filter the string with acting for AND
strs_for_data = ["tgt=", "act=", "err=", "~="]       # strings in the array for data that we want to get data for
plotting = True                                      # boolean, whether we will plot data in graphs
file_name_input = "input.txt"
file_name_output = "output.txt"
str_divider = "\t"

# internal variables
heading = "index" + str_divider
for data_string in strs_for_data:
   heading += data_string
   heading += str_divider
values_temp = []

# variables for plotting
index = 0
index_array = []
value_arrays = [[],[],[],[],[],[],[],[],[],[]]

"""
\brief function to get a value for a data in the given string
\param param parameter that we want to get data for
      string string to find the value for the data in
\return value value for the data if it exist, 0 if not
"""
def get_number_for( param, string ):
   pattern = r'{}(-?[0-9]+(?:\.[0-9]+)?)'.format(param)
   match = re.search(pattern, line)
   if match:
       return match.group(1)
   return 0

"""
\brief function to get the value array
\param num_array the number of array we want to push the data in
\return an array in value_arrays, which is a global variable
"""
def get_value_array( num_array ):
   return value_arrays[ num_array ]

"""
\brief function to insert a value into the array
\param num_array the number of array we want to push the data in
      value the value that we want to push in the array
"""
def insert_value_to_array( num_array, value ):
   array = get_value_array( num_array )
   array.append( value )
 
# main routine : parse from log lines if any
fr = open( file_name_input, 'r' )
fw = open( file_name_output, 'w' )
fw.write( heading )
fw.write( "\n" )
   
lines = fr.readlines()
for line in lines:
   # skip the line if it's not in interest
   skip = False
   for string in strs_to_filter_by:
       index_start = line.find( string )
       if index_start < 0:
           skip = True
           break
   if skip == True:
       continue

   # start parsing data that we want to get from the current line        
   index += 1
   values_temp.clear()
   for data_string in strs_for_data:
       values_temp.append( get_number_for( data_string, line ) )
   
   # get data ready if plotting is used
   if plotting == True:
       index_array.append( index )
       array_index = 0
       for value in values_temp:
           insert_value_to_array( array_index, value )
           array_index += 1

   # make up a new line for data formatted in what we want and write into the output file
   line_parsed = str( index ) + str_divider
   for value in values_temp:
       temp = str( value )
       line_parsed += temp
       line_parsed += str_divider        
   fw.write( line_parsed )
   fw.write( "\n" )    
   
fr.close()
fw.close()

# plotting
if plotting == True:
   plt.figure( figsize=(10,5) )

   array_index = 0
   for data_string in strs_for_data:
       c = 'C' + str( array_index + 1 )
       plt.plot( index_array, get_value_array( array_index ), c, linewidth=2.0, label=data_string )
       array_index += 1

   plt.xlabel( 'time [index]' )
   plt.ylabel( 'error level [count]' )
   plt.legend( loc='lower right' )

   ## fileout
   today = date.today()
   output_graph_name = "output_graph_" + today.strftime("%Y%m%d") +".png"
   plt.savefig( output_graph_name )
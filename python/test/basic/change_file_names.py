#######################################################################################################
# brief  This module is in order to convert .mp3 file name for ebs 입트영
# detail
#  A .mp3 file name for it is basicallly like this: "입트영 24년 2월 1일.mp3"
#  And title looks like this: "1 Thu. Car Door Dings / 차량 문콕 테러"
#  Note that in the titles.txt, anything for 'Weekly Review' must be removed manually beforehand.
#  And if one of these formats is changed, the code should be changed accordingly to correctly parse
#  and change file names.
#######################################################################################################

import os

dir_path = os.getcwd()

# get only files names ending .mp3
file_names = os.listdir(dir_path)
mp3_file_names = [file for file in file_names if file.endswith(".mp3")]

# get the title file
title_file = "titles.txt"
title_dir_path = dir_path + "\\" + title_file

# get the file names from the title file
title_name_vec = []
f_title_file = open(title_dir_path, 'r', encoding='utf-8')
lines = f_title_file.readlines()
for line in lines:
    #print(line)
    
    # create a new file name
    line = line.replace("\n","")
    line = line.replace(" / ","_")
    index_slicing = line.find('.') + 2
    new_title = line[index_slicing:]
    
    # append it to the vector
    #print(new_title)
    title_name_vec.append(new_title)

for file in mp3_file_names:
    words = file.split(' ')

    # make up 'OO' (월)    
    day = words[2].replace("월","")
    count = len(day)
    if count < 2:
        day = '0'+day
    
    # make up '00' (일)
    date = words[3].replace("일.mp3","")
    count = len(date)
    if count < 2:
        date = '0'+date

    # get initial file names
    file_new = day + '-' + date + '_'
    #print( file_new )
    
    dir_path_old = dir_path + "\\" + file
    dir_path_new = dir_path + "\\" + file_new
        
    # change the file name with the new one
    os.rename( dir_path_old, dir_path_new  )
    
#get new mp3 files again.    
file_names = os.listdir(dir_path)
mp3_file_names = [file for file in file_names if file.endswith("_")] 

i = 0
for file in mp3_file_names:
    file_name_final = file + title_name_vec[i] + ".mp3"

    dir_path_old = dir_path + "\\" + file
    dir_path_new = dir_path + "\\" + file_name_final
    
    # change the file name
    os.rename( dir_path_old, dir_path_new  )
    i += 1



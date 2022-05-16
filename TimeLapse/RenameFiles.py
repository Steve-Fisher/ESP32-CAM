import os

FILE_START = 'IMG'
FILE_END = '.jpg'

files = [ f for f in os.listdir('.') if f.startswith(FILE_START) and f.endswith(FILE_END)]
nums = [int(file.replace(FILE_END, '').replace(FILE_START, '')) for file in files]
nums.sort()

i = 1
for num in nums:
    old_name = FILE_START + str(num) + FILE_END
    new_name = "NEW_" + FILE_START + str(i) + FILE_END
    os.rename(old_name, new_name)
    print(f'{old_name} becomes {new_name}')
    i=i+1
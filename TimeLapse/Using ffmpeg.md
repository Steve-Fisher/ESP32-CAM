# Using ffmpeg

## Example
Taken from: https://hamelot.io/visualization/using-ffmpeg-to-convert-a-set-of-images-into-a-video/

>Using ffmpeg to convert a set of images into a video
>
>Apr 5, 2016
>
>Original 2012-11-16, Updated 2016-04-05: cleanup and information about overlaying images.
>
>When using ffmpeg to compress a video, I recommend using the libx264 codec, from experience it has given me excellent quality for small video sizes. I have noticed that different versions of ffmpeg will produce different output file sizes, so your mileage may vary.
>
>To take a list of images that are padded with zeros (pic0001.png, pic0002.png…. etc) use the following command:
>
>```shell
>ffmpeg -r 60 -f image2 -s 1920x1080 -i pic%04d.png -vcodec libx264 -crf 25  -pix_fmt yuv420p test.mp4
>```
>... where the %04d means that zeros will be padded until the length of the string is 4 i.e 0001…0020…0030…2000 and so on. If no padding is needed use something similar to pic%d.png or %d.png.
>
>-r is the framerate (fps)
>
>-crf is the quality, lower means better quality, 15-25 is usually good
>
>-s is the resolution
>
>-pix_fmt yuv420p specifies the pixel format, change this as needed the file will be output (in this case) to: test.mp4

## First used example

ffmpeg doesn't have an option to start image sequences from anything other than 1!

```shell
ffmpeg -r 25 -f image2 -s 1600x1200 -i IMG%d.jpg -vcodec libx264 -crf 25  -pix_fmt yuv420p -vf "transpose=1, scale=300:400" test1.mp4
```

* %d specifies non-padded number (1, 2, 3 ... 8, 9, 10, 11, 12 ... 98, 99, 100, 101, 102 ...)
* It produces a blank black video without the pix_fmt
* -vf "transpose=1" rotates the video clockwise.  (See https://ostechnix.com/how-to-rotate-videos-using-ffmpeg-from-commandline/)
* -s appears to be the input resolution (not output).  Use scale in -vf to re-scale output video.  Note, the width:height is *after* the transpose.

To get the correct image file sequence number requires re-naming the files.  Windows sorts files alphaetically, whereas we need to sort based on some (incorrect) sequence number.  Therefore, need to use a python script to do this renaming:
```python
import os

FILE_START = 'IMG'
FILE_END = '.jpg'

files = [ f for f in os.listdir('.') if f.startswith(FILE_START) and f.endswith(FILE_END)]
nums = [int(file.replace(FILE_END, '').replace(FILE_START, '')) for file in files]
nums.sort()

i = 1
for num in nums:
    old_name = FILE_START + str(num) + FILE_END
    new_name = FILE_START + str(i) + FILE_END
    os.rename(old_name, new_name)
    print(f'{old_name} becomes {new_name}')
    i=i+1
``` 
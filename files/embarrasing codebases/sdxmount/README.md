# SDxMount

## Synopsis
 SDxMount is a small C++ program to mount that pesky drive that always appears with a different drive letter. It is easily configured through source in the SDxMount.cpp file.


## What it does
 Mounts a drive based on its size as shown in 'lsblk'.


## How it works
 it checks lsblk for drives of a certain size, then mounts the first one it finds to the specified mount point

## Usage
 once it is compiled, all you need to do is run it, and maybe type in the password you would use if you were invoking a command using sudo. It will find the drive name, and mount it automatically. If you are having issues, see Troubleshooting

## Configuration
 All configuration is done through modifying the source code. This may seem intimidating, but I promise I laid it out an easy to understand format

 When opening 'SDxMount.cpp' in a text editor, the three configuration options are right at the top:
 - Drive Size
 - Drive Name Size(++)
 - Partition Number (of the desired drive)
 - Desired Mount Point 

### Drive Size:
 To find the drive size you need for your drive run 'lsblk' in a terminal. Determine which drive you want to be mounted, and put in the size exactly as it is listed in quotation marks.

### Drive Name Size:
 The Drive Name Size is the number of characters in the drive name PLUS ONE! The drive will always have the same number of letters, even if they are different letters, so if the drive is currently sdb, that is three characters, adding one makes it 4, so input 4. If the drive is currently nvme0n1, that is 7 characters, so input 8.  

### Partition Number:
 Input the desired partition number as it shows up in 'lsblk'. Don't forget to put it in quotation marks. Some drives, such as NVME drives, show partition numbers with p1, p2, etc. rather than the normal 1, 2, 3. Keep this in mind, as if your drive is like this you will need to specify p1 rather than just 1.

### Mount Point:
 This is the folder that the drive will be mounted to. Choose a folder that doesn't contain anything, and type the complete directory here. If the path is really long, you might need to make the number in COMMAND_BUFFER_SIZE larger.

--

## Compilation:
### Dependencies: 
 - GCC (compilation)
 - grep (runtime)
 - lsblk (runtime)
 Although a precompiled example is given, you will almost certainly need to compile it yourself to make it useful to you. The compilation process is extremely easy. Ensure GCC is installed and run make. Since everything is contained in one .cpp file, you can also run g++ SDxMount.cpp and compile it that way.

## Troubleshooting:
 > make sure the partition number given before compiling is both on the desired drive and mountable
 
 > make sure the configured size is exactly the same as it is listed in LSBLK. If it is even slightly off, it will not work
 
 > make sure the mount point is a real folder that exists on your system
 
 > run ```lsblk | grep [specified size]``` in a terminal, replacing [specified size] with the size you specified in the program. if the beginning of the first line outputs as the name of your drive, then it is working properly. If it doesn't, then you either input the size incorrectly, or either lsblk or grep does not work in a way on your system that allows this program to run properly

 > try mounting the drive manually using the drive name you got from the above command, the drive number you specified, and the mount point you specified: ```sudo mount [drive name][drive number] [path to mounting folder]```. If this works to mount your drive, you almost definitely made a typo in your configuration

 If none of these work, go back to Configuration and reread the specifications provided there
## License:
SDxMount is free to use, modify, and distribute in any way and for any purpose. I wouldn't reccommend trying to sell it, but I don't care to try and stop you. It is originally distributed with no warranty, and no gaurantees. 

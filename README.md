mp3hash
=======
Command line tool to calculate the hash of the music data in mp3 files
(without id3v1 & id3v2 metadata). Useful to find dupes with e.g. different
genre names.

Build
-----
	gcc -o mp3hash mp3hash.c md5/md5.c

3rd party source
----------------
The md5 implementation was written by L Peter Deutsch and published under
zlib/libpng License. It's hosted on sourceforge:
[http://sourceforge.net/projects/libmd5-rfc](http://sourceforge.net/projects/libmd5-rfc)

Usage
-----
To calculate the hash for a single file:   

	mp3hash <filename>

You can also pass one or more filenames via stdin:

	ls *.mp3 | mp3hash @

Output will be md5 hash, length of the id3v2 data and the filename.

Example
-------
	sptim$ ls -l *.mp3
	-rw-r--r-- 1 sptim wheel 44180672 May 14 17:02 52 Folge 52_ Vorschau 2011.mp3
	-rw-r--r-- 1 sptim wheel 44094414 May 14 17:03 Folge 52_ Vorschau 2011.mp3

Obviously these files are different. So let's check with mp3hash:

	sptim$ ls *.mp3 | mp3hash @
	1b6a96bbf3070fd2770e622245037386 000086258 52 Folge 52_ Vorschau 2011.mp3
	1b6a96bbf3070fd2770e622245037386 000000000 Folge 52_ Vorschau 2011.mp3

As you can see their hashes match, but the second file has no id3v2 data.


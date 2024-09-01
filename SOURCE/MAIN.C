#include <stdio.h>
#include "INCLUDE/FILEIO.H"
#include "INCLUDE/DDS.H"

const char *ProgramName = "QuickDDS";

int main(int argc, char **argv)
{
	if (argc < 2)
	{
		printf(
		"Incorrect usage:\n"
		"Usage: %s.exe {File}\n"
		"Or drag file onto %s.exe\n", ProgramName, ProgramName);
		
		return -1;
	}

	PATHINFO pi;
	GET_PATHINFO(&pi, argv[1]);

	FILE *fh = OPEN_READ(pi.FilePath, F_BINARY);

	puts("\n"
		"QuickDDS\n"
		"A simple DDS Extraction tool, developed by smakdev\n"
		"\n"
		"\n"
		"  Offset   Size          Name        FourCC\n"
		"---------------------------------------------------\n");

	char OUTPATH[260];
	sprintf(OUTPATH, "%s/%s", pi.FileDirectory, pi.FileName);

	char OUTFILE[260];

	U32 NFILES = 0;

	while (0 == IS_EOF(fh))
	{
		char cur = READU8(fh);
		if (cur == 'D')
		{
			if (!('D' == READU8(fh)))
				continue;
			if (!('S' == READU8(fh)))
				continue;
			if (!(' ' == READU8(fh)))
				continue;

			SIZE pos = GET_POSITION(fh) - 4;

			CREATE_DIRECTORY(OUTPATH);

			sprintf(OUTFILE, "%s/%u.dds", OUTPATH, ++NFILES);

			IMAGE_DDS IMAGE;
			int ret = ReadDDSFile(fh, &IMAGE);

			if (0 == ret)
				WriteDDS(&IMAGE, OUTFILE);
			else
				WriteDDS_UNKHDR(fh, &IMAGE, OUTFILE);


			printf("  %08zx %-10zu %4u.dds       (%.4s)\n", pos, IMAGE.SIZE, NFILES, (char *)&IMAGE.HEADER.ddspf.dwFourCC);
		}
	}

	puts("---------------------------------------------------\n");

	(NFILES) 
		? printf("Found %u DDS files\n", NFILES) 
		: printf("No DDS files found\n");



	EXIT(0);
}
#!/bin/bash

filename=hwshgen.c
exec=hwshgen.run

echo "#include <stdlib.h>
#include <stdio.h>

int main () {
    const char *prog = \"filename=hw.c\nexec=hw.run\n\necho \\\"#include <stdio.h>\n\nint main () {\n\tprintf(\\\\\\\"Hello world!\\\n\\\\\\\");\n\treturn 0;\n}\\\" > \$filename\n\ngcc -o \$exec \$filename\n./\$exec\n\";
    FILE* out = fopen(\"hwsh.sh\", \"wr\");
    fprintf(out, \"%s\", prog);
    
    fclose(out);
    system(\"bash hwsh.sh\");
    return 0;
}" > $filename

gcc -o $exec $filename
./$exec

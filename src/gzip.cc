#include <stdio.h>
#include <iostream>
#include <string.h>
#include "zlib.h"

//To remove / Compile with -lz
int main()
{
  bool accept_encoding_gzip = true;
  if (accept_encoding_gzip)
  {
    char answer_uncompressed[256] = "Salut Salut Salut Salut Salut Salut!";
    char answer_compressed[256];
  
    std::cout << "Uncompressed answer: " << answer_uncompressed << std::endl;
    std::cout << "Uncompressed size: " << strlen(answer_uncompressed) << std::endl;
    std::cout << "\n-------------------\n\n" << std::endl;

    z_stream basic;
    basic.zalloc = Z_NULL;
    basic.zfree = Z_NULL;
    basic.opaque = Z_NULL;

    basic.avail_in = (uInt)strlen(answer_uncompressed)+1;
    basic.next_in = (Bytef *)answer_uncompressed;
    basic.avail_out = (uInt)sizeof(answer_compressed);
    basic.next_out = (Bytef *)answer_compressed;

    deflateInit2(&basic, 1, Z_DEFLATED, 15, 9, Z_DEFAULT_STRATEGY);
    deflate(&basic, Z_FINISH);
    deflateEnd(&basic);

    std::cout << "Compressed answer: " << answer_compressed << std::endl;
    std::cout << "Compressed size: " << strlen(answer_compressed) << std::endl;
  } 
    return 0;
}



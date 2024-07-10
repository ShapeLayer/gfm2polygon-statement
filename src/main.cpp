#include <iostream>
#include <string>
#include <cstdlib>
#include <cerrno>

#include <cmark-gfm.h>

#include "polygon.hpp"

#define false 0
#define true 1

void print_usage() {
  std::cout << "Usage:  md2pst [FILE*]" << std::endl;
}

void print_document(cmark_node *document, int options, cmark_parser *parser) {
  std::string result = cmark_render_pstatement(document, options);
  std::cout << result << std::endl;
}

int main(int argc, char *argv[]) {
  int i = 0;

  int nfiles = 0;
  int *files;
  char buffer[4096];
  size_t bytes;

  int options = CMARK_OPT_DEFAULT;
  cmark_node *document;
  cmark_parser *parser;

  int exit_code = EXIT_FAILURE;

  files = (int*)calloc(argc, sizeof(*files));

  for (i = 1; i < argc; i++) {
    files[nfiles++] = i;
    if ((strcmp(argv[i], "--help") == 0) || (strcmp(argv[i], "-h") == 0)) {
      print_usage();
      goto success;
    }
  }

#if DEBUG
  parser = cmark_parser_new(options);
#else
  parser = cmark_parser_new_with_mem(options, cmark_get_arena_mem_allocator());
#endif

  for (i = 0; i < nfiles; i++) {
    FILE *fp = fopen(argv[files[i]], "rb");
    if (fp == NULL) {
      std::cerr << "Failed to open file " << argv[files[i]] << ": " << strerror(errno) << std::endl;
      goto failure;
    }

    while ((bytes = fread(buffer, 1, sizeof(buffer), fp)) > 0) {
      cmark_parser_feed(parser, buffer, bytes);
      if (bytes < sizeof(buffer)) {
        break;
      }
    }

    fclose(fp);
  }

  document = cmark_parser_finish(parser);
  if (!document)
    goto failure;
  print_document(document, options, parser);

success:
  exit_code = EXIT_SUCCESS;

failure:

#if DEBUG
  if (parser)
  cmark_parser_free(parser);

  if (document)
    cmark_node_free(document);
#else
  cmark_arena_reset();
#endif

  free(files);
  return exit_code;
}

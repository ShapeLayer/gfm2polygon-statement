#include <iostream>
#include <stdlib.h>
#include <string.h>

#include <cmark-gfm.h>

#include "polygon.hpp"

struct render_state {
  std::string content;
  int indent;
};

void S_render_node(cmark_node *node, cmark_event_type ev_type, struct render_state *state, int options) {
  if (node == NULL) return;

  bool entering = (ev_type == CMARK_EVENT_ENTER);

  switch (cmark_node_get_type(node)) {
  case CMARK_NODE_DOCUMENT:
    break;
  case CMARK_NODE_BLOCK_QUOTE:
    if (entering) {
      state->content.append("\\epigraph{\\it{\n");
    } else {
      state->content.append("}}\n");
    }
    break;
  case CMARK_NODE_LIST:
    if (entering) {
      if (cmark_node_get_list_type(node) == CMARK_ORDERED_LIST) {
        state->content.append("\\begin{enumerate}\n");
      } else {
        state->content.append("\\begin{itemize}\n");
      }
    } else {
      if (cmark_node_get_list_type(node) == CMARK_ORDERED_LIST) {
        state->content.append("\\end{enumerate}\n");
      } else {
        state->content.append("\\end{itemize}\n");
      }
    }
    break;
  case CMARK_NODE_ITEM:
    if (entering) {
      state->content.append("\\item ");
    } else {
      state->content.append("\n");
    }
    break;
  case CMARK_NODE_CODE_BLOCK:
  case CMARK_NODE_HTML_BLOCK:
    state->content.append("\\begin{lstlisting}\n");
    state->content.append(cmark_node_get_literal(node));
    state->content.append("\\end{lstlisting}\n");
    break;
  case CMARK_NODE_CUSTOM_BLOCK:
    break;
  case CMARK_NODE_PARAGRAPH:
    if (entering) {
      state->content.append("\n");
    } else {
      state->content.append("\n\n");
    }
    break;
  case CMARK_NODE_HEADING:
    if (entering) {
      int level = cmark_node_get_heading_level(node);
      state->content.append("\\");
      {
        switch (level) {
          case 1:
            state->content.append("Huge"); break;
          case 2:
            state->content.append("huge"); break;
          case 3:
            state->content.append("LARGE"); break;
          case 4:
            state->content.append("Large"); break;
          case 5:
            state->content.append("large"); break;
        }
      }
      state->content.append("{");
    } else {
      state->content.append("}\n");
    }
    break;
  case CMARK_NODE_THEMATIC_BREAK:
    break;
  case CMARK_NODE_FOOTNOTE_DEFINITION:
    break;
  case CMARK_NODE_TEXT:
    if (entering) {
      state->content.append(cmark_node_get_literal(node));
    }
    break;
  case CMARK_NODE_SOFTBREAK:
    if (entering) {
      state->content.append("\n");
    }
    break;
  case CMARK_NODE_LINEBREAK:
    if (entering) {
      state->content.append("\\\\\n");
    }
    break;
  case CMARK_NODE_CODE:
  case CMARK_NODE_HTML_INLINE:
    state->content.append("\\texttt{");
    state->content.append(cmark_node_get_literal(node));
    state->content.append("}");
    break;
  case CMARK_NODE_CUSTOM_INLINE:
    break;
  case CMARK_NODE_EMPH:
    if (entering) {
      state->content.append("\\textit{");
    } else {
      state->content.append("}");
    }
    break;
  case CMARK_NODE_STRONG:
    if (entering) {
      state->content.append("\\textbf{");
    } else {
      state->content.append("}");
    }
    break;
  case CMARK_NODE_LINK: {
    const char *url = cmark_node_get_url(node);
    const char *title = cmark_node_get_title(node);
    if (entering) {
      if (title != NULL) {
        state->content.append("\\href{");
      } else {
        state->content.append("\\url{");
      }
      if (url != NULL) {
        state->content.append(url);
      }
      state->content.append("}");
      state->content.append("{");
    } else {
      state->content.append("}");
    }
    break;
  }
  case CMARK_NODE_IMAGE: {
    if (entering) {
      const char *url = cmark_node_get_url(node);
      const char *title = cmark_node_get_title(node);

      state->content.append("\\begin{center}\n");
      if (url != NULL) {
        state->content.append("\\includegraphics{");
        state->content.append(url);
        state->content.append("} \\\\ \n");
      }
      if (title != NULL) {
        state->content.append("\\small{");
        state->content.append(title);
        state->content.append("}");
      }
      state->content.append("\\end{center}\n");
      break;
    }
  }
  default:
    break;
  }

}

std::string cmark_render_pstatement(cmark_node *root, int options) {
  std::string result;
  cmark_event_type ev_type;
  cmark_node *cur;
  cmark_iter *iter = cmark_iter_new(root);
  struct render_state state = {result, 0};

  while ((ev_type = cmark_iter_next(iter)) != CMARK_EVENT_DONE) {
    cur = cmark_iter_get_node(iter);
    S_render_node(cur, ev_type, &state, options);
  }

  cmark_iter_free(iter);
  return state.content;
}

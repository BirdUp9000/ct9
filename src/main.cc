#include <filesystem>
#include <fstream>
#include <iostream>

#include "../include/ct9/Trie.h"

#if BUILD_GUI
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#define POSX 100
#define POSY 100
#define WIDTH 200
#define HEIGHT 50
#define BORDER 1
#define TEXTBOX_X 5
#define TEXTBOX_Y 30

static Display* dpy;
static Window root;
static int screen;
#endif

int main() {
  Trie t{};
  std::ifstream file("../tests/words.txt");

  if (!file.is_open()) {
    std::cerr << "File opening error.\n";
    return EXIT_FAILURE;
  }

  std::string line;
  while (std::getline(file, line)) {
    t.insert(line);
  }

  std::cout << t.autocomplete("").size() << std::endl;

#if BUILD_GUI
  // Creating window
  dpy = XOpenDisplay(nullptr);
  if (dpy == nullptr) {
    std::cerr << "Failed to open X display" << '\n';
    return EXIT_FAILURE;
  }

  screen = DefaultScreen(dpy);
  root = RootWindow(dpy, screen);

  // Colors
  XColor color_gray, color_blue, ignore;
  XAllocNamedColor(dpy, DefaultColormap(dpy, screen), "Web Gray", &color_gray, &ignore);
  XAllocNamedColor(dpy, DefaultColormap(dpy, screen), "Light Blue", &color_blue, &ignore);

  const Window win =
      XCreateSimpleWindow(dpy, root, POSX, POSY, WIDTH, HEIGHT, BORDER, BlackPixel(dpy, screen), color_blue.pixel);
  XSelectInput(dpy, win, ExposureMask | KeyPressMask);
  XMapWindow(dpy, win);

  // Creating inputbox
  GC gc = XCreateGC(dpy, win, 0, nullptr);
  XSetForeground(dpy, gc, BlackPixel(dpy, screen));
  std::string inputText;

  // Choosing font
  XFontStruct* fontInfo = XLoadQueryFont(dpy, "12x24");
  if (fontInfo == nullptr) {
    fontInfo = XLoadQueryFont(dpy, "9x15");
    if (fontInfo == nullptr) {
      fontInfo = XLoadQueryFont(dpy, "fixed");
      if (fontInfo == nullptr) {
        std::cerr << "No suitable font found! Exiting...\n";
        exit(EXIT_FAILURE);
      }
    }
  }
  XSetFont(dpy, gc, fontInfo->fid);

  // Result of autocompletion
  std::queue<std::string> words;

  bool running = true;
  while (running) {
    XEvent event;
    XNextEvent(dpy, &event);

    switch (event.type) {
      case Expose:
        XClearWindow(dpy, win);
        XSetForeground(dpy, gc, BlackPixel(dpy, screen));
        XDrawString(dpy, win, gc, TEXTBOX_X, TEXTBOX_Y, inputText.c_str(), static_cast<int>(inputText.size()));
        break;

      case KeyPress: {
        KeySym keysym;
        char buffer[32];
        int bytes = XLookupString(&event.xkey, buffer, sizeof(buffer), &keysym, nullptr);

        if (keysym == XK_BackSpace) {
          if (!inputText.empty()) {
            inputText.pop_back();
          }
        } else if (keysym == XK_Escape) {
          running = false;
          break;
        } else if (keysym == XK_Return) {
          t.insert(inputText);
          inputText.clear();
        } else if (keysym == XK_Tab) {
          auto result = t.autocomplete(inputText, 1);
          if (!result.empty()) {
            const auto& completed_str = result.front();
            inputText = completed_str;
            XSetForeground(dpy, gc, BlackPixel(dpy, screen));
            XDrawString(dpy, win, gc, TEXTBOX_X, TEXTBOX_Y, completed_str.c_str(), static_cast<int>(completed_str.size()));
          }
        } else if (keysym == XK_space) {
          break;
        } else {
          if (bytes > 0) {
            inputText.append(buffer, bytes);
          }
        }
        XClearWindow(dpy, win);
        auto result = t.autocomplete(inputText, 1);
        if (!result.empty()) {
          const auto& completed_str = result.front();
          XSetForeground(dpy, gc, color_gray.pixel);
          XDrawString(dpy, win, gc, TEXTBOX_X, TEXTBOX_Y, completed_str.c_str(), static_cast<int>(completed_str.size()));
        }

        XSetForeground(dpy, gc, BlackPixel(dpy, screen));
        XDrawString(dpy, win, gc, TEXTBOX_X, TEXTBOX_Y, inputText.c_str(), static_cast<int>(inputText.size()));

        break;
      }

      default:
        break;
    }
  }

  if (fontInfo) {
    XFreeFont(dpy, fontInfo);
    fontInfo = nullptr;
  }

  XFreeGC(dpy, gc);
  XUnmapWindow(dpy, win);
  XDestroyWindow(dpy, win);
  XCloseDisplay(dpy);
#else
  printf("Running without GUI.\n");
#endif

  file.close();
  return EXIT_SUCCESS;
}

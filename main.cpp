#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

#include <cstring>
#include <iostream>

using namespace std;

struct Node {
    Node *parent = nullptr, *next = nullptr, *prev = nullptr, *last = nullptr, *first = nullptr;
    inline void setParent(Node *value) {
        if (parent) {
            if (prev) prev->next = next;
            else parent->first = next;
            if (next) next->prev = prev;
            else parent->last = prev;
        }

        next = prev = nullptr;

        if (parent = value) {
            if (prev = parent->last) {
                prev->next = this;
            } else {
                parent->first = this;
            }
            parent->last = this;
        }
    }
};

int main(int argc, char **argv) {
    auto input = argv[1];
    auto output = argv[2];
    auto precision = 0xff / 0x1f;
    auto width = 0, height = 0;
    auto map = stbi_load(input, &width, &height, nullptr, 1);
    auto count = width * height;
    auto image = new unsigned int[count];
    auto nodes = new Node[count];
    auto odin = new Node; // All-father ...

    auto sameType = [&](int i, int j)-> bool {
        return (map[i]>map[j]?map[i]-map[j]:map[j]-map[i]) < precision;
    };

    getchar();
    puts("Setting nodes...");

    for (int i = 0; i < count; ++i) {
        Node *hold = nullptr;
        Node *root = nullptr;

        if (i % width) {
            int j = i - 1;
            if (sameType(i, j)) {
                hold = nodes[j].parent;
                root = hold->parent;
            }
        }

        if (i >= width) {
            int j = i - width;
            if (sameType(i, j)) {
                if (hold) {
                    if (root != nodes[j].parent->parent) {
                        for (Node *hold = root->first, *next; hold; hold = next) {
                            next = hold->next;
                            hold->setParent(nodes[j].parent->parent);
                        }
                        root->setParent(nullptr);
                        delete root;
                    }
                } else {
                    hold = nodes[j].parent;
                    root = hold->parent;
                }
            }
        }

        if (root == nullptr) {
            root = new Node;
            root->setParent(odin);
        }

        if (hold == nullptr) {
            hold = new Node;
            hold->setParent(root);
        }

        nodes[i].setParent(hold);
    }

    puts("Setting nodes done...");
    getchar();
    puts("Drawing image...");

    for (Node *root = odin->first, *next; root; root = next) {
        unsigned int color = 0xff << 24 | rand();
        for (Node *hold = root->first, *next; hold; hold = next) {
            for (Node *item = hold->first; item; item = item->next) {
                image[item - nodes] = color;
            }
            next = hold->next;
            delete hold;
        }
        next = root->next;
        delete root;
    }

    puts("Drawing image done...");

    stbi_write_png(output, width, height, 4, image, 0);

    delete odin;
    delete[] nodes;
    free(map);
    return 0;
}

#pragma once

#define DEFAULT_FILE "index.html"

struct tag_attribute {
    char *name;
    char *value;
};

struct html_tag {
    char* name;
    struct tag_attribute** attributes;
    int attributes_count;
    struct html_tag* parent;
    struct html_tag** childs;
    int childs_count;
    char* content;
};

int get_html(char* path,char**html);
struct html_tag* html_parser(char* html_string);
int test_parser (char* path);
int create_html(struct html_tag* html,char* code);
struct html_tag* find_tag(struct html_tag* html,char* tag_name);
int remove_tag(struct html_tag* html);
int deref_tag(struct html_tag* tag);
int getcode(struct html_tag* html);
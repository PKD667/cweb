#pragma once

struct html_tag {
    char* name;
    struct html_tag* parent;
    struct html_tag** childs;
    int childs_count;
    char* content;
    int test;
};

int get_html(char* path,char**html);
struct html_tag* html_parser(char* html_string);
int test_parser (char* path);
int print_html(struct html_tag* html);
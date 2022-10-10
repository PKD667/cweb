#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/html.h"

struct html_tag* html_parser(char* html_string)
{
    printf("Html string is %s\n",html_string);


    struct html_tag* current_tag = NULL;
    printf("current_tag address is %p\n",current_tag);

    printf("looping through the string\n");
    int in_tag = 0;
    for (int i = 0; i < strlen(html_string); i++)
    {
        printf("current char is %c and we are in %s , current tag is %s\n",html_string[i],in_tag ? "tag" : "content",current_tag ? current_tag->name : "NULL");


        if (html_string[i] == '<')
        {
            //we are in a tag
            in_tag = 1;
            //check if we are closing a tag
            if (html_string[i+1] == '/')
            {
                // check if the closing tag is valid
                printf("closing tag for %s is %.*s\n",current_tag->name,(int)strlen(current_tag->name),html_string+i+2);
                if (strncmp(html_string+i+2,current_tag->name,strlen(current_tag->name)) == 0)
                {
                    //print skipped chars
                    printf("skipping chars : %.*s\n",(int)strlen(current_tag->name)+2,html_string+i);
                    i += strlen(current_tag->name)+2;
                    //the closing tag is valid
                    //check if the closing tag is the root tag
                    if (current_tag->parent == NULL)
                    {
                        //we are closing the root tag
                        //we are done parsing
                        printf("we are done parsing\n");
                        break;
                    }
                    else
                    {
                        //we are closing a child tag
                        //set the current tag to the parent tag
                        current_tag = current_tag->parent;
                        printf("current_tag address is %p\n",current_tag);
                    }

                }
                else
                {
                    //the closing tag is not valid
                    //we have an error
                    printf("error: closing tag is not valid\n");
                    return NULL;
                }
            }
            else
            {
                //we are opening a tag
                //allocate a new tag
                printf("allocating a new tag\n");

                struct html_tag* new_tag = calloc(1,sizeof(struct html_tag));
                //set the parent
                new_tag->parent = current_tag;
                if (new_tag->parent != NULL)
                {
                    //add the new tag to the childs of the current tag
                    current_tag->childs_count++;
                    current_tag->childs = realloc(current_tag->childs,current_tag->childs_count*sizeof(struct html_tag*));
                    current_tag->childs[current_tag->childs_count-1] = new_tag;
                }
                //set the new tag as the current tag
                current_tag = new_tag;
            }
        }
        else if (html_string[i] == '>')
        {
            //we are out of a tag
            printf("we are out of a tag\n");
            in_tag = 0;
        }
        else
        {
            //we are in a tag name or content
            //check if we are in a tag name
            if (current_tag->name == NULL)
            {
                printf("found a tag name in %d at %p\n",current_tag->test,current_tag);
                //we are in a tag name

                //we are in a opening tag
                //allocate the name
                current_tag->name = calloc(64,sizeof(char));
                //copy the name
                int j = 0;
                while (html_string[i] != ' ' && html_string[i] != '>')
                {
                    current_tag->name[j] = html_string[i];
                    i++;
                    j++;
                }
                printf("tag name is %s\n",current_tag->name);
                if (html_string[i] == ' ')
                {
                    in_tag = 1;
                }
                else if (html_string[i] == '>')
                {
                    in_tag = 0;
                }
                else {
                    printf("error: unexpected char\n");
                    return NULL;
                }
                
            }
            else if (in_tag)
            {
                // we are in a tag (not content)
                // do nothing
            }
            else
            {
                //we are in a tag content
                //allocate the content
                // TODO: find a better aloocating system
                current_tag->content = calloc(256,sizeof(char));
                //copy the content
                int j = 0;
                while (html_string[i] != '<')
                {
                    current_tag->content[j] = html_string[i];
                    i++;
                    j++;
                }
                //go back one character
                i--;
            }
        }
    }
    printf("=== DONE ===\n");
    printf("html name is %s\n",current_tag->name);
    
    return current_tag;

    return 0;

}

int test_parser (char* path)
{
    // test the html parser
    // open test.html
    FILE* test_html = fopen("test.html","r");
    if (test_html == NULL)
    {
        printf("Error opening test.html\n");
        return 1;
    }
    // get the size of the file
    fseek(test_html,0,SEEK_END);
    long test_html_size = ftell(test_html);
    fseek(test_html,0,SEEK_SET);
    // read the file
    char* test_html_string = malloc(test_html_size);
    fread(test_html_string,1,test_html_size,test_html);
    // close the file
    fclose(test_html);
    // parse the html
 
    struct html_tag* html = html_parser(test_html_string);
    printf("Html address is %p\n",html);

    //print the html
    print_html(html);

    return 0;

}

// recursive print html func
int print_html(struct html_tag* html)
{

    printf("tag name is %s\n",html->name);
    printf("tag content is %s\n",html->content);
    for (int j = 0;j < html->childs_count;j++)
    {
        print_html(html->childs[j]);
    }
    return 0;

}

int get_html(char* path,char**html)
{

    return 0;
}
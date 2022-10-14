#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../include/html.h"
#include "../include/main.h"

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
                    in_tag = 0;
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
                // parse attribute
                printf("parsing attribute\n");
                //allocate a new attribute
                current_tag->attributes = realloc(current_tag->attributes,(current_tag->attributes_count+1)*sizeof(struct tag_attribute*));
                current_tag->attributes[current_tag->attributes_count] = calloc(1,sizeof(struct tag_attribute));
                //allocate the name
                current_tag->attributes[current_tag->attributes_count]->name = calloc(64,sizeof(char));
                //copy the name
                int j = 0;
                while (html_string[i] != '=')
                {
                    current_tag->attributes[current_tag->attributes_count]->name[j] = html_string[i];
                    i++;
                    j++;
                }
                printf("attribute name is %s\n",current_tag->attributes[current_tag->attributes_count]->name);
                //skip the '='
                i++;

                //allocate the value
                current_tag->attributes[current_tag->attributes_count]->value = calloc(64,sizeof(char));
                //copy the value
                j = 0;
                // remove the quotes
                i++;
                while (html_string[i] != '"')
                {
                    current_tag->attributes[current_tag->attributes_count]->value[j] = html_string[i];
                    i++;
                    j++;
                }
                printf("attribute value is %s\n",current_tag->attributes[current_tag->attributes_count]->value);
                //increment the attributes count
                current_tag->attributes_count++;

                continue;

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
    char* returned_html = calloc(1024,sizeof(char));
    create_html(html,returned_html);
    printf("returned html is %s\n",returned_html);


    return 0;

}

int create_html(struct html_tag* html,char* code)
{
    printf("creating html for %s\n",html->name);
    // reursively loop over tags 
    // and create the html code
    // add the tag name
    strcat(code,"<");
    strcat(code,html->name);
    // add atributes
    for (int i = 0; i < html->attributes_count; i++)
    {
        strcat(code," ");
        strcat(code,html->attributes[i]->name);
        strcat(code,"=\"");
        strcat(code,html->attributes[i]->value);
        strcat(code,"\"");
    }
    strcat(code,">");
    // add the content
    strcat(code,html->content);
    // add the childs
    for (int i = 0; i < html->childs_count; i++)
    {
        create_html(html->childs[i],code);
    }
    // add the closing tag
    strcat(code,"</");
    strcat(code,html->name);
    strcat(code,">");
    return 0;
}
struct html_tag* find_tag(struct html_tag* html,char* tag_name)
{
    // find a tag in the html
    // return the tag
    for (int i = 0; i < html->childs_count; i++)
    {
        printf("checking tag '%s' = '%s'\n",html->childs[i]->name,tag_name);
        if (strcmp(html->childs[i]->name,tag_name) == 0)
        {
            printf("found tag '%s'\n",tag_name);
            return html->childs[i];
        }
        else
        {
            printf("tag '%s' not found, checking childs\n",tag_name);
            struct html_tag * res = find_tag(html->childs[i],tag_name);
            if (res != NULL)
            {
                return res;
            }
        }
    }
    return NULL;
}
int remove_tag(struct html_tag* html)
{
    // remove a tag from the html
    // free the memory
    // free the childs

    // remove the refence from the parent
    for (int i = 0; i < html->parent->childs_count; i++)
    {
        if (html->parent->childs[i] == html)
        {
            html->parent->childs[i] = NULL;
        }
    }
    // decrement parent childs count
    html->parent->childs_count--;
    for (int i = 0; i < html->childs_count; i++)
    {
        remove_tag(html->childs[i]);
    }
    // free the attributes
    for (int i = 0; i < html->attributes_count; i++)
    {
        free(html->attributes[i]->name);
        free(html->attributes[i]->value);
        free(html->attributes[i]);
    }
    // free the tag
    free(html->name);
    free(html->content);
    free(html->attributes);
    free(html->childs);
    free(html);
    return 0;
}

int deref_tag(struct html_tag* tag)
{
    // deref a tag
    // remove the tag from the html
    // and free the memory
    // but keep the childs
    // remove the refence from the parent
    for (int i = 0; i < tag->parent->childs_count; i++)
    {
        if (tag->parent->childs[i] == tag)
        {
            tag->parent->childs[i] = NULL;
        }
    }
    // decrement parent childs count
    tag->parent->childs_count--;
    return 0;
}

int get_html(char* path,char** html_str)
{
    // open test.html
    // test the html parser
    // open test.html
    FILE* test_html = fopen(DEFAULT_FILE,"r");
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

    getcode(html);

    //print the html
    *html_str = calloc(1024,sizeof(char));
    create_html(html,*html_str);
    printf("returned html is %s\n",*html_str);

    return 0;

}

int getcode(struct html_tag* html)
{
    printf("getting code from %s\n",html->name);

    // find a <c?> tag
    struct html_tag* ctag = find_tag(html,"c?");
    if (ctag == NULL)
    {
        printf("error: no c? tag found\n");
        return 1;
    }
    //remove the c? tag
    deref_tag(ctag);
    // get the code
    char* code = ctag->content;
    // run the code
    runc(code,html);
    return 0;
}

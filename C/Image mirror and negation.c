#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SIZE 41
#define SIZE2 51

struct image_t {
  char type[3];
  int **ptr;
  int width;
  int height;
};

void destroy_2d_array(int **ptr, int height)
{
    if(!ptr)return;
    if(height<=0)
    {
        free(ptr);
        return;
    }
    for(int i=0; i<height; i++)
        if(*(ptr+i)!=NULL)free(*(ptr+i));
    free(ptr);
}

int **create_2d_array(int width, int height)
{
    if(width<=0 || height<=0)return NULL;
    int **ptr;
    ptr=malloc(height*sizeof(int*));
    if(!ptr)return NULL;

    for(int i=0; i<height; i++)
    {
        *(ptr+i)=malloc(width*sizeof(int));
        if(!*(ptr+i))
        {
            destroy_2d_array(ptr, i);
            return NULL;
        }
    }
    return ptr;
}

struct image_t* image_flip_vertical(const struct image_t *m1)
{
    if(!m1 || !m1->ptr || m1->width<=0 || m1->height<=0)return NULL;

    int **ptr=create_2d_array(m1->width, m1->height);
    if(!ptr)return NULL;
    struct image_t *m2=malloc(sizeof(struct image_t));
    if(!m2)
    {
        destroy_2d_array(ptr, m2->height);
        return NULL;
    }

    for(int i=0; i<m1->height; i++)
    {
        for(int j=0; j<m1->width; j++)
            *(*(ptr+i)+m1->width-1-j)=*(*(m1->ptr+i)+j);
    }

    m2->ptr=ptr;
    m2->width=m1->width;
    m2->height=m1->height;
    strcpy(m2->type, m1->type);

    return m2;
}
struct image_t* image_flip_horizontal(const struct image_t *m1)
{
    if(!m1 || !m1->ptr || m1->width<=0 || m1->height<=0)return NULL;

    int **ptr=create_2d_array(m1->width, m1->height);
    if(!ptr)return NULL;
    struct image_t *m2=malloc(sizeof(struct image_t));
    if(!m2)
    {
        destroy_2d_array(ptr, m2->height);
        return NULL;
    }

    for(int i=0; i<m1->height; i++)
    {
        for(int j=0; j<m1->width; j++)
            *(*(ptr+m1->height-1-i)+j)=*(*(m1->ptr+i)+j);
    }

    m2->ptr=ptr;
    m2->width=m1->width;
    m2->height=m1->height;
    strcpy(m2->type, m1->type);

    return m2;
}
struct image_t* image_negate(const struct image_t *m1)
{
    if(!m1 || !m1->ptr || m1->width<=0 || m1->height<=0)return NULL;

    int **ptr=create_2d_array(m1->width, m1->height);
    if(!ptr)return NULL;
    struct image_t *m2=malloc(sizeof(struct image_t));
    if(!m2)
    {
        destroy_2d_array(ptr, m1->height);
        return NULL;
    }

    for(int i=0; i<m1->height; i++)
    {
        for(int j=0; j<m1->width; j++)
            *(*(ptr+i)+j)=255-*(*(m1->ptr+i)+j);
    }

    m2->ptr=ptr;
    m2->width=m1->width;
    m2->height=m1->height;
    strcpy(m2->type, m1->type);

    return m2;
}

void destroy_image(struct image_t **m)
{
    if(!m || !*m || (*m)->height<=0)return;
    destroy_2d_array((*m)->ptr, (*m)->height);
    free(*m);
}
struct image_t* load_image_t(const char *filename, int *err_code)
{
    if(!filename)
    {
        if(err_code)*err_code=1;
        return NULL;
    }

    FILE *fp;
    fp=fopen(filename, "r");
    if(!fp)
    {
        if(err_code)*err_code=2;
        return NULL;
    }
    struct image_t *m1;
    m1=malloc(sizeof(struct image_t));
    if(!m1)
    {
        if(err_code)*err_code=4;
        fclose(fp);
        return NULL;
    }

    if(!fscanf(fp, "%2s", m1->type) || strcmp("P2", m1->type) || strlen( m1->type)<=0)
    {
        free(m1);
        fclose(fp);
        if(err_code)*err_code=3;
        return NULL;
    }

    int max_pixel_value;
    if(fscanf(fp, "%d%d%d", &(m1->width), &(m1->height), &max_pixel_value)!=3 || max_pixel_value<0 || max_pixel_value>255 || m1->height<=0 || m1->width<=0)
    {
        free(m1);
        fclose(fp);
        if(err_code)*err_code=3;
        return NULL;
    }

    int **ptr=create_2d_array(m1->width, m1->height);
    if(!ptr)
    {
        free(m1);
        fclose(fp);
        if(err_code)*err_code=4;
        return NULL;
    }

    int i=0,j=0,count=0;
    while(i<m1->height)
    {
        int value=0;
        if(fscanf(fp, "%d", &value))count++;
        if(feof(fp))break;

        if(value<0 || value>max_pixel_value)
        {
            destroy_2d_array(ptr, m1->height);
            free(m1);
            fclose(fp);
            if(err_code)*err_code=3;
            return NULL;
        }

        *(*(ptr+i)+j)=value;
        j++;
        if(j>=m1->width)
        {
            j=0;
            i++;
        }
    }
    if(count!=(m1->width)*(m1->height))
    {
        destroy_2d_array(ptr, m1->height);
        free(m1);
        fclose(fp);
        if(err_code)*err_code=3;
        return NULL;
    }

    m1->ptr=ptr;

    fclose(fp);
    if(err_code)*err_code=0;
    return m1;
}

int save_image_t(const char * filename, const struct image_t *m1)
{
    if(!filename || !m1 || !m1->type || !m1->ptr || m1->width<=0 || m1->height<=0 || strcmp(m1->type, "P2"))
        return 1;

    FILE *fp;
    fp=fopen(filename, "w");
    if(!fp)return 2;

    fputs(m1->type, fp);
    fprintf(fp, "\n%d %d\n", m1->width, m1->height);

    fprintf(fp, "%d\n", 255);
    int **ptr=m1->ptr;
    for(int i=0; i<m1->height; i++)
    {
        for(int j=0; j<m1->width; j++)
            fprintf(fp, "%d ", *(*(ptr+i)+j));
        if(i+1<m1->height)putc('\n', fp);
    }

    fclose(fp);
    return 0;
}

int main()
{
    char *filename;
    filename=malloc(SIZE*sizeof(char));
    if(!filename)
    {
        printf("Failed to allocate memory");
        return 4;
    }
    printf("Podaj nazwe pliku: ");
    scanf("%40s", filename);

    int err_code;
    struct image_t *m1;
    m1=load_image_t(filename, &err_code);
    if(err_code==2 || err_code==1)
    {
        printf("File not found");
        free(filename);
        return 2;
    }
    if(err_code==3)
    {
        printf("File corrupted");
        free(filename);
        return 3;
    }
    if(err_code==4)
    {
        printf("Failed to allocate memory");
        free(filename);
        return 4;
    }

    struct image_t *m2;
    m2=image_flip_horizontal(m1);
    if(!m2)
    {
        printf("Failed to allocate memory");
        free(filename);
        destroy_image(&m1);
        return 4;
    }
    char *filename_mirror_h;
    filename_mirror_h=malloc(SIZE2*sizeof(char));
    if(!filename_mirror_h)
    {
        printf("Couldn't create file\n");
        destroy_image(&m2);
    }
    else
    {
        *filename_mirror_h='\0';
        strcat(filename_mirror_h, "mirror_h_");
        strcat(filename_mirror_h, filename);
        if(save_image_t(filename_mirror_h, m2)!=0)
            printf("Couldn't create file\n");
        else printf("File saved\n");
        destroy_image(&m2);
        free(filename_mirror_h);
    }

    struct image_t *m3;
    m3=image_flip_vertical(m1);
    if(!m3)
    {
        printf("Failed to allocate memory");
        free(filename);
        destroy_image(&m1);
        return 4;
    }
    char *filename_mirror_v;
    filename_mirror_v=malloc(SIZE2*sizeof(char));
    if(!filename_mirror_v)
    {
        printf("Couldn't create file\n");
        destroy_image(&m3);
    }
    else
    {
        *filename_mirror_v='\0';
        strcat(filename_mirror_v, "mirror_v_");
        strcat(filename_mirror_v, filename);
        if(save_image_t(filename_mirror_v, m3)!=0)
            printf("Couldn't create file\n");
        else printf("File saved\n");
        destroy_image(&m3);
        free(filename_mirror_v);
    }


    struct image_t *m4;
    m4=image_negate(m1);
    if(!m4)
    {
        printf("Failed to allocate memory");
        free(filename);
        destroy_image(&m1);
        return 4;
    }
    char *filename_invert;
    filename_invert=malloc(SIZE2*sizeof(char));
    if(!filename_invert)
    {
        printf("Couldn't create file\n");
        destroy_image(&m4);
    }
    else
    {
        *filename_invert='\0';
        strcat(filename_invert, "invert_");
        strcat(filename_invert, filename);
        if(save_image_t(filename_invert, m4)!=0)
            printf("Couldn't create file\n");
        else printf("File saved\n");
        destroy_image(&m4);
        free(filename_invert);
    }


    free(filename);
    destroy_image(&m1);
    return 0;
}


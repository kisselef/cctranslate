//
// Created by Oleg Kisselef (olegkisselef at gmail dot com) on 6/17/15
//

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "config.h"

#define CLO_UNRECOGNIZED "unrecognized command line option \"%s\"\n"
#define CLO_ILLEGAL_VALUE "illegal command line option \"%s\" value \"%s\"\n"

void cct_config_init(cct_config *config)
{
    config->input_filename = NULL;
    config->output_filename = NULL;
    config->source_type = CCT_SOURCE_SUBRIP;
    config->sink_type = CCT_SINK_SUBRIP;
    config->translator_type = CCT_TRANSLATOR_GOOGLE;
    config->list_langs = 0;
    config->google_api_key = NULL;
    config->source_lang = NULL;
    config->langs = NULL;
    config->show_help = 0;
    config->ccextractor_url = NULL;
}

void cct_config_cleanup(cct_config *config)
{
    free(config->input_filename);
    free(config->output_filename);
    free(config->google_api_key);
    free(config->langs);
    free(config->source_lang);
    free(config->ccextractor_url);
}

int is_option_value_valid(char *option)
{
    if (strchr(option, '%')) {
        fprintf(stderr, "is_option_value_valid: option value can't contain percent %% symbol\n");
        return 0;
    }
    return 1;
}

cct_status cct_parse_cl_args(cct_config *config, int argc, char * const argv[])
{
    char *cl_option = NULL,
            *cl_value = NULL;

    for (int i = 1; i < argc; i++) {
        cl_option = strtok(argv[i], "=");
        if (!cl_option) { //single
            fprintf(stderr, "parse_cl_params: can't parse first argument\n");
            return CCT_FATAL;
        } else {
            if (!strcmp(cl_option, "-i") || !strcmp(cl_option, "--input")) {
                cl_value = strtok(NULL, "=");
                if (!is_option_value_valid(cl_value)) {
                    fprintf(stderr, CLO_ILLEGAL_VALUE, "--input", cl_value);
                    return CCT_FATAL;
                }
                config->input_filename = strdup(cl_value);
            } else if (!strcmp(cl_option, "-o") || !strcmp(cl_option, "--output")) {
                cl_value = strtok(NULL, "=");
                if (!is_option_value_valid(cl_value)) {
                    fprintf(stderr, CLO_ILLEGAL_VALUE, "--output", cl_value);
                    return CCT_FATAL;
                }
                config->output_filename = strdup(cl_value);
            } else if (!strcmp(cl_option, "-k") || !strcmp(cl_option, "--key")) {
                cl_value = strtok(NULL, "=");
                if (!is_option_value_valid(cl_value)) {
                    fprintf(stderr, CLO_ILLEGAL_VALUE, "--key", cl_value);
                    return CCT_FATAL;
                }
                config->google_api_key = strdup(cl_value);
            } else if (!strcmp(cl_option, "-l") || !strcmp(cl_option, "--langs")) {
                cl_value = strtok(NULL, "=");
                if (!is_option_value_valid(cl_value)) {
                    fprintf(stderr, CLO_ILLEGAL_VALUE, "--langs", cl_value);
                    return CCT_FATAL;
                }
                config->langs = strdup(cl_value);
            } else if (!strcmp(cl_option, "-h") || !strcmp(cl_option, "--help")) {
                config->show_help = 1;
            } else if (!strcmp(cl_option, "--list-langs")) {
                config->list_langs = 1;
            } else if (!strcmp(cl_option, "-s") || !strcmp(cl_option, "--source")) {
                cl_value = strtok(NULL, "=");
                if (!is_option_value_valid(cl_value)) {
                    fprintf(stderr, CLO_ILLEGAL_VALUE, "--source", cl_value);
                    return CCT_FATAL;
                }
                if (!strcmp(cl_value, "extractor")) {
                    config->source_type = CCT_SOURCE_CCEXTRACTOR;
                } else if (!strcmp(cl_value, "subrip")) {
                    config->source_type = CCT_SOURCE_SUBRIP;
                } else {
                    fprintf(stderr, CLO_ILLEGAL_VALUE, "--source", cl_value);
                }
            } else if (!strcmp(cl_option, "-u") || !strcmp(cl_option, "--extractor-url")) {
                if (!is_option_value_valid(cl_value)) {
                    fprintf(stderr, CLO_ILLEGAL_VALUE, "--extractor-url", cl_value);
                    return CCT_FATAL;
                }
                config->ccextractor_url = strdup(cl_value);
            } else {
                fprintf(stderr, CLO_UNRECOGNIZED, cl_option);
                return CCT_FATAL;
            }
        }
    }
    return CCT_OK;
}

void usage()
{
    printf("cctranslate is a part of ccextractor project http://ccextractor.org/\n");
    printf("version %d.%d.%d\n", CCTRANSLATE_VERSION_MAJOR,
           CCTRANSLATE_VERSION_MINOR, CCTRANSLATE_VERSION_MAINT);
    printf("usage: cctranslate args\n");
    printf("args syntax: --name=value, -n=value, --novaluearg\n");
    printf("\t-i, --input\t\tSubrip source filename]\n");
    printf("\t-o, --output\t\tOutput filename\n");
    printf("\t-k, --key\t\tGoogle Translate API key\n");
    printf("\t-l, --langs\t\tTarget languages (comma-separated, e.g. -l=fr,it)\n");
    printf("\t-s, --source\t\tSource type. Possible values: extractor, subrip\n");
    printf("\t-u, --extractor-url\t\tUrl of ccextractor sharing service (e.g. tcp://localhost:3269\n");
    printf("\t--list-langs\t\tList available languages\n");
    printf("\t\n");
    printf("\t-h, --help\t\tShow this help\n");
}
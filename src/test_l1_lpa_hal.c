/*
* If not stated otherwise in this file or this component's LICENSE file the
* following copyright and licenses apply:*
* Copyright 2023 RDK Management
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/
 
/**
* @file test_l1_lpa_hal.c
* @page lpa_hal Level 1 Tests
*
* ## Module's Role
* This module includes Level 1 functional tests (success and failure scenarios).
* This is to ensure that the lpa_hal APIs meet the requirements across all vendors.
*
* **Pre-Conditions:**  None@n
* **Dependencies:** None@n
*
* Ref to API Definition specification documentation : [halSpec.md](../../../docs/halSpec.md)
*/

#include <ut.h>
#include <ut_log.h>
#include "lpa_hal.h"
#include <stdlib.h>
#include "cJSON.h"
#include <string.h>
#include <ctype.h>
#include<stdbool.h>

char** iccid = NULL;
int num_iccid = 0;

/**function to read the json config file and return its content as a string
 *IN : json file name
 *OUT : content of json file as string
 **/
static char *read_file(const char *filename)
{
    FILE *file = NULL;
    long length = 0;
    char *content = NULL;
    size_t read_chars = 0;

    /* open in read mode */
    file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Please place lpa_config file ,where your binary is placed\n");
        exit(1);
    }
    else
    {
        /* get the length */
        if (fseek(file, 0, SEEK_END) == 0)
        {
            length = ftell(file);
            if (length > 0)
            {
                if (fseek(file, 0, SEEK_SET) == 0)
                {
                    /* allocate content buffer */
                    content = (char *)malloc((size_t)length + sizeof(""));
                    if (content != NULL)
                    {
                        /* read the file into memory */
                        read_chars = fread(content, sizeof(char), (size_t)length, file);
                        if ((long)read_chars != length)
                        {
                            free(content);
                            content = NULL;
                        }
                        else
                            content[read_chars] = '\0';
                    }
                }
            }
            else
            {
                printf("lpa_config file is empty. please add configuration\n");
                exit(1);
            }
        }
        fclose(file);
    }
    return content;
}

/**function to read the json config file and return its content as a json object
 *IN : json file name
 *OUT : content of json file as a json object
 **/
static cJSON *parse_file(const char *filename)
{
    cJSON *parsed = NULL;
    char *content = read_file(filename);
    parsed = cJSON_Parse(content);

    if (content != NULL)
    {
        free(content);
    }

    return parsed;
}

/* Free memory allocated for iccid */
void freeiccid(void)
{
    int i = 0;
    if (iccid != NULL)
    {
        for (i = 0; i < num_iccid; i++)
        {
            free(iccid[i]);
        }
        free(iccid);
    }
}


int get_iccid(void)
{
    char configFile[] = "./lpa_config";
    cJSON *value = NULL;
    cJSON *json = NULL;
    cJSON *item = NULL;
    int i = 0;

    UT_LOG("Checking iccid...  \n");
    json = parse_file(configFile);
    if (json == NULL)
    {
        printf("Failed to parse config\n");
        return -1;
    }
    value = cJSON_GetObjectItem(json, "iccid");
    // null check and object is Array, value->valuestring
    if ((value != NULL) && (cJSON_IsArray(value)))
    {
        num_iccid = cJSON_GetArraySize(value);
        printf("Number of iccid : %d \n", num_iccid);

        // Allocate memory for iccid
        iccid = (char **)malloc(num_iccid * sizeof(char *));
        if (iccid == NULL)
        {
            printf("Memory allocation failed\n");
            cJSON_Delete(json);
            return -1;
        }
        cJSON_ArrayForEach(item, value)
        {
            if (i < num_iccid && cJSON_IsString(item))
            {
                // Allocate memory for each string and copy the content
                iccid[i] = (char *)malloc((strlen(item->valuestring) + 1) * sizeof(char));
                if (iccid[i] == NULL)
                {
                    printf("Memory allocation failed\n");
                    freeiccid();
                    cJSON_Delete(json);
                    return -1;
                }

                strcpy(iccid[i], item->valuestring);
                i++;
            }
        }
    }
    // Free cJSON object as it is no longer needed
    cJSON_Delete(json);
    return 0;
}

/**
* @brief This test validates the eSIM download profile functionality
*
* The function cellular_esim_download_profile_from_smds is tested for a valid server details, to ensure the ability of the API to download profile successfully from the given server. @n
*  @n
* **Test Group ID:** Basic: 01 @n
* **Test Case ID:** 001 @n
* **Priority:** High @n
* @n
* **Pre-Conditions:** None @n
* **Dependencies:** None @n
* **User Interaction:** If user chose to run the test in interactive mode, then the test case has to be selected via console. @n
* @n
* **Test Procedure:**@n
* | Variation / Step | Description | Test Data |Expected Result |Notes |
* | :----: | :---------: | :----------: |:--------------: | :-----: |
* | 01 | Invoking cellular_esim_download_profile_from_smds() function with valid input | smds = "oem-smds-json.demo.gemalto.com"  | RETURN_OK | Should be successful |
*/
void test_l1_lpa_hal_positive1_cellular_esim_download_profile_from_smds(void)
{
    UT_LOG("Entering test_l1_lpa_hal_positive1_cellular_esim_download_profile_from_smds... \n");
    char *smds = "oem-smds-json.demo.gemalto.com"; 
    UT_LOG("Invoking lpa_hal_positive1_cellular_esim_download_profile_from_smds with valid smds");
    int result = cellular_esim_download_profile_from_smds(smds);
    UT_LOG("cellular_esim_download_profile_from_smds Return result: %d", result);
    UT_ASSERT_EQUAL(result, RETURN_OK);
    UT_LOG("Exiting test_l1_lpa_hal_positive1_cellular_esim_download_profile_from_smds... \n");
}

/**
* @brief Test the cellular_esim_download_profile_from_smds function with NULL smds
*
* This testcase is designed to test the scenario where the cellular_esim_download_profile_from_smds function is called with NULL smds . @n
* @n
* **Test Group ID:** Basic: 01 @n
* **Test Case ID:** 002 @n
* **Priority:** High @n
* @n
* **Pre-Conditions:** None @n
* **Dependencies:** None @n
* **User Interaction:** If user chose to run the test in interactive mode, then the test case has to be selected via console
* @n
* **Test Procedure:**@n
* | Variation / Step | Description | Test Data |Expected Result |Notes |
* | :----: | --------- | ---------- |-------------- | ----- |
* | 01 | Call cellular_esim_download_profile_from_smds function with NULL smds | smds = NULL | RETURN_ERR | should be failure |
*/

void test_l1_lpa_hal_negative1_cellular_esim_download_profile_from_smds(void)
{
    UT_LOG("Entering test_l1_lpa_hal_negative1_cellular_esim_download_profile_from_smds... \n");
    char *smds = NULL;
    UT_LOG("Invoking cellular_esim_download_profile_from_smds with NULL smds");
    int result = cellular_esim_download_profile_from_smds(smds);
    UT_LOG("cellular_esim_download_profile_from_smds Return result: %d", result);
    UT_ASSERT_EQUAL(result, RETURN_ERROR);
    UT_LOG("Exiting test_l1_lpa_hal_negative1_cellular_esim_download_profile_from_smds... \n");
}

/**
* @brief Tests the eSIM download profile function with an empty smds
*
* This test invokes the cellular_esim_download_profile_from_smds() function with an empty smds string. @n 
* @n
* **Test Group ID:** Basic: 01@n
* **Test Case ID:** 003@n
* **Priority:** High@n
* @n
* **Pre-Conditions:** None@n
* **Dependencies:** None@n
* **User Interaction:** If user chose to run the test in interactive mode, then the test case has to be selected via console.
* @n
* **Test Procedure:**@n
* | Variation / Step | Description  | Test Data |Expected Result |Notes |
* | :----: | --------- | ---------- |-------------- | ----- |
* | 01 | Invoke cellular_esim_download_profile_from_smds() with empty smds string. | smds = "" | RETURN_ERROR | Should be Failed |
*/
void test_l1_lpa_hal_negative2_cellular_esim_download_profile_from_smds(void)
{
    UT_LOG("Entering test_l1_lpa_hal_negative2_cellular_esim_download_profile_from_smds... \n");
    char *smds= "";   
    UT_LOG("Invoking cellular_esim_download_profile_from_smds with empty string smds.");
    int result = cellular_esim_download_profile_from_smds(smds);
    UT_LOG("cellular_esim_download_profile_from_smds Return result: %d", result);
    UT_ASSERT_EQUAL(result, RETURN_ERROR);
    UT_LOG("Exiting test_l1_lpa_hal_negative2_cellular_esim_download_profile_from_smds... \n");
}

/**
* @brief Tests the eSIM download profile function with an empty smds
*
* This test invokes the cellular_esim_download_profile_from_smds() function with an invalid smds string. @n
* @n
* **Test Group ID:** Basic: 01@n
* **Test Case ID:** 004@n
* **Priority:** High@n
* @n
* **Pre-Conditions:** None@n
* **Dependencies:** None@n
* **User Interaction:** If user chose to run the test in interactive mode, then the test case has to be selected via console. @n
* @n
* **Test Procedure:**@n
* | Variation / Step | Description  | Test Data |Expected Result |Notes |
* | :----: | --------- | ---------- |-------------- | ----- |
* | 01 | Invoke cellular_esim_download_profile_from_smds() with Invalid smds string. | smds = "o6m-smds-j#on.e@mo.g@malto.com" | RETURN_ERROR | should be fail |
*/
void test_l1_lpa_hal_negative3_cellular_esim_download_profile_from_smds(void)
{
    UT_LOG("Entering test_l1_lpa_hal_negative3_cellular_esim_download_profile_from_smds... \n");
    char *smds = "o6m-smds-j#on.e@mo.g@malto.com";   
    UT_LOG("Invoking cellular_esim_download_profile_from_smds with alphaneumaric input smds.");
    int result = cellular_esim_download_profile_from_smds(smds);
    UT_LOG("cellular_esim_download_profile_from_smds Return result: %d", result);
    UT_ASSERT_EQUAL(result, RETURN_ERROR);
    UT_LOG("Exiting test_l1_lpa_hal_negative3_cellular_esim_download_profile_from_smds... \n");
}

/**
* @brief Test the cellular_esim_download_profile_from_defaultsmdp API with valid smdp
*
* This test aims to validate if the cellular_esim_download_profile_from_defaultsmdp. @n
* @n
* **Test Group ID:** Basic: 01@n
* **Test Case ID:** 005@n
* **Priority:** High@n
* @n
* **Pre-Conditions:** None@n
* **Dependencies:** None@n
* **User Interaction:** If user chose to run the test in interactive mode, then the test case has to be selected via console. @n
* @n
* **Test Procedure:**@n
* | Variation / Step | Description | Test Data |Expected Result |Notes |
* | :----: | --------- | ---------- |-------------- | ----- |
* | 01 | Invoking the API cellular_esim_download_profile_from_defaultsmdp with valid smdp | smdp = "smdp-plus.test.gsma.com" | RETURN_OK | Should be successful |
*/
void test_l1_lpa_hal_positive1_cellular_esim_download_profile_from_defaultsmdp(void)
{
    UT_LOG("Entering test_l1_lpa_hal_positive1_cellular_esim_download_profile_from_defaultsmdp...");
    char *smdp = "smdp-plus.test.gsma.com";
    UT_LOG("Invoking cellular_esim_download_profile_from_defaultsmdp with valid smdp");
    int result = cellular_esim_download_profile_from_defaultsmdp(smdp);
    UT_LOG("cellular_esim_download_profile_from_defaultsmdp Return result: %d", result);
    UT_ASSERT_EQUAL(result, RETURN_OK);
    UT_LOG("Exiting test_l1_lpa_hal_positive1_cellular_esim_download_profile_from_defaultsmdp...");
}

/**
* @brief This test verifies the response when cellular_esim_download_profile_from_defaultsmdp function is invoked with NULL smdp.
*
* The objective of this test is to make sure that the cellular_esim_download_profile_from_defaultsmdp function returns an error when smdp is NULL. @n
* @n
* **Test Group ID:** Basic: 01 @n
* **Test Case ID:** 006 @n
* **Priority:** High @n
* @n
* **Pre-Conditions:** None @n
* **Dependencies:** None @n
* **User Interaction:** If user chose to run the test in interactive mode, then the test case has to be selected via console @n
* @n
* **Test Procedure:**@n
* | Variation / Step | Description | Test Data |Expected Result |Notes |
* | :----: | --------- | ---------- |-------------- | ----- |
* | 01 | Invoking the API cellular_esim_download_profile_from_defaultsmdp function with NULL smdp | smdp = NULL | RETURN_ERR | Should be fail |
*/

void test_l1_lpa_hal_negative1_cellular_esim_download_profile_from_defaultsmdp(void) 
{
    UT_LOG("Entering test_l1_lpa_hal_negative1_cellular_esim_download_profile_from_defaultsmdp...");
    char *smdp = NULL;
    UT_LOG("Invoking cellular_esim_download_profile_from_defaultsmdp with null smdp");
    int result = cellular_esim_download_profile_from_defaultsmdp(smdp);
    UT_LOG("cellular_esim_download_profile_from_defaultsmdp Return result: %d", result);
    UT_ASSERT_EQUAL(result, RETURN_ERROR);
    UT_LOG("Exiting test_l1_lpa_hal_negative1_cellular_esim_download_profile_from_defaultsmdp...");
}

/**
 * @brief Test to validate the cellular_esim_download_profile_from_defaultsmdp API when given an empty string as input.
 *
 * This unit test checks the behavior of the cellular_esim_download_profile_from_defaultsmdp API when it is provided with an empty string as input. @n
 * @n
 * **Test Group ID:** Basic: 01 @n
 * **Test Case ID:** 007 @n
 * **Priority:** High @n
 *  @n
 * **Pre-Conditions:** None @n
 * **Dependencies:** None @n
 * **User Interaction:** If user chose to run the test in interactive mode, then the test case has to be selected via console @n
 *  @n
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data |Expected Result |Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Invoke the API cellular_esim_download_profile_from_defaultsmdp with empty string | smdp = "" | RETURN_ERR | Should be Fail |
 */
void test_l1_lpa_hal_negative2_cellular_esim_download_profile_from_defaultsmdp(void) 
{
    UT_LOG("Entering test_l1_lpa_hal_negative2_cellular_esim_download_profile_from_defaultsmdp...");
    char *smdp = "";
    UT_LOG("Invoking cellular_esim_download_profile_from_defaultsmdp with empty string smdp.");
    int result = cellular_esim_download_profile_from_defaultsmdp(smdp);
    UT_LOG("cellular_esim_download_profile_from_defaultsmdp Return result: %d", result);
    UT_ASSERT_EQUAL(result, RETURN_ERROR);
    UT_LOG("Exiting test_l1_lpa_hal_negative2_cellular_esim_download_profile_from_defaultsmdp...");
}
/**
 * @brief Test to validate the cellular_esim_download_profile_from_defaultsmdp API .
 *
 * This unit test checks the behavior of the cellular_esim_download_profile_from_defaultsmdp API when it is provided with invalid value as input.  @n
 * @n
 * **Test Group ID:** Basic: 01 @n
 * **Test Case ID:** 008 @n
 * **Priority:** High @n
 *  @n
 * **Pre-Conditions:** None @n
 * **Dependencies:** None @n
 * **User Interaction:** If user chose to run the test in interactive mode, then the test case has to be selected via console @n
 *  @n
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data |Expected Result |Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Invoke the API cellular_esim_download_profile_from_defaultsmdp with Invalid string | smdp = "d#vice8efaultSMD@Address=smdp-plus.test.g9ma.com" | RETURN_ERR | Should be Fail |
 */
void test_l1_lpa_hal_negative3_cellular_esim_download_profile_from_defaultsmdp(void) 
{
    UT_LOG("Entering test_l1_lpa_hal_negative3_cellular_esim_download_profile_from_defaultsmdp...");
    char *smdp = "d#vice8efaultSMD@Address=smdp-plus.test.g9ma.com";
    UT_LOG("Invoking cellular_esim_download_profile_from_defaultsmdp with alphaneumaric input smdp.");
    int result = cellular_esim_download_profile_from_defaultsmdp(smdp);
    UT_LOG("cellular_esim_download_profile_from_defaultsmdp Return result: %d", result);
    UT_ASSERT_EQUAL(result, RETURN_ERROR);
    UT_LOG("Exiting test_l1_lpa_hal_negative3_cellular_esim_download_profile_from_defaultsmdp...");
}

/**
 * @brief Unit testing with CUnit framework for cellular_esim_get_profile_info API.
 *
 * In this test case, we are testing the scenario when the cellular_esim_get_profile_info API is invoked with valid parameters.  @n
 * @n
 * **Test Group ID:** Basic: 01 @n
 * **Test Case ID:** 009 @n
 * **Priority:** High @n
 *  @n
 * **Pre-Conditions:** None @n
 * **Dependencies:** None @n
 * **User Interaction:** If user chose to run the test in interactive mode, then the test case has to be selected via console. @n
 *  @n
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Invoke the API cellular_esim_get_profile_info with valid inputs | profile_list = valid pointer, nb_profiles = valid buffer | RETURN_OK | should be successful |
*/
void test_l1_lpa_hal_positive1_cellular_esim_get_profile_info(void)
{
    UT_LOG("Entering test_l1_lpa_hal_positive1_cellular_esim_get_profile_info ...");

    int nb_profiles = 0;
    eSIMProfileStruct *profile_list = NULL;
       
    UT_LOG("Invoking cellular_esim_get_profile_info with valid parameters.");
    int result = cellular_esim_get_profile_info(&profile_list, &nb_profiles);
    UT_LOG("cellular_esim_get_profile_info Return result: %d", result);
    UT_ASSERT_EQUAL(result, RETURN_OK);
    UT_LOG("checking whether the pointer \"profile_list\" is null pointer or not");
    UT_ASSERT_PTR_NOT_NULL(profile_list);
    if(result == RETURN_OK && nb_profiles > 0)
    {
        UT_LOG("cellular_esim_get_profile_info for nb_profiles :%d",nb_profiles);
        for(int i = 0;i<nb_profiles;i++)
        {
            UT_LOG("profile : %d iccid :%s",i+1,(profile_list+i)->iccid);
            UT_LOG("profile : %d profileName :%s",i+1,(profile_list+i)->profileName);
            UT_LOG("profile : %d profileState :%d",i+1,(profile_list+i)->profileState);
            bool iccid_flag = false;
            for(int j = 0; j<num_iccid ;j++)
            {
                if(!strcmp((profile_list+i)->iccid,iccid[j]))
                {
                    iccid_flag = true;
                    break;
                }

            }
            if(iccid_flag)
            {
                UT_LOG("profile : %d iccid is valid : %s",i+1,(profile_list+i)->iccid);
                UT_PASS("valid iccid");
            }
            else
            {
                UT_LOG("profile : %d iccid is invalid : %s",i+1,(profile_list+i)->iccid);
                UT_FAIL("invalid iccid");
            }
            
            if(!strcmp((profile_list+i)->profileName, "Xfinity Mobile") || !strcmp((profile_list+i)->profileName ,"Comcast") || !strcmp((profile_list+i)->profileName, "CRTC"))
            {
                UT_LOG("profile : %d cellular_esim_get_profile_info profileName value is %s which is a valid value",i+1,(profile_list+i)->profileName);
                UT_PASS("cellular_esim_get_profile_info profile_list of profileName value validation success");
            }
            else
            {
                UT_LOG("profile : %d cellular_esim_get_profile_info profileName value is %s which is a invalid value",i+1,(profile_list+i)->profileName);
                UT_FAIL("cellular_esim_get_profile_info profile_list of profileName value  validation failed");
            }
            if(((profile_list+i)->profileState == 00) || ((profile_list+i)->profileState == 01))
            {
                UT_LOG("profile : %d cellular_esim_get_profile_info profileState value is %s which is a valid value",i+1,(profile_list+i)->profileState);
                UT_PASS("cellular_esim_get_profile_info profile_list of profileName value validation success");
            }
            else
            {
                UT_LOG("profile : %d cellular_esim_get_profile_info profileState value is %s which is a invalid value",i+1,(profile_list+i)->profileState);
                UT_FAIL("cellular_esim_get_profile_info profile_list of profileState value  validation failed");
            }
            if((nb_profiles >= 0) && (nb_profiles <= 2147483647))
            {
                UT_LOG("profile : %d cellular_esim_get_profile_info nb_profiles value is %d which is a valid value",i+1,nb_profiles);
                UT_PASS("cellular_esim_get_profile_info nb_profiles of profileName value validation success");
            }
            else
            {
                UT_LOG("profile : %d cellular_esim_get_profile_info nb_profiles value is %d which is a invalid value",i+1,nb_profiles);
                UT_FAIL("cellular_esim_get_profile_info nb_profiles of profileState value  validation failed");
            }
        }
    }
    if(nb_profiles == 0)
    {
        UT_LOG("No profiles available");
    }
    UT_LOG("Exiting test_l1_lpa_hal_positive1_cellular_esim_get_profile_info ...");
}

/**
* @brief This tests cellular_esim_get_profile_info function with NULL parameters.
*
* In this test, the aim is to validate the robustness of the API cellular_esim_get_profile_info by invoking it with NULL parameters. @n
* @n
* **Test Group ID:** Basic: 01 @n
* **Test Case ID:** 010 @n
* **Priority:** High @n
* @n
* **Pre-Conditions:** None @n
* **Dependencies:** None @n
* **User Interaction:** If user chose to run the test in interactive mode, then the test case has to be selected via console. @n
*  @n
* **Test Procedure:**@n
* | Variation / Step | Description | Test Data |Expected Result |Notes |
* | :----: | --------- | ---------- |-------------- | ----- |
* | 01 | Invoke the API cellular_esim_get_profile_info with NULL profile_list | profile_list = NULL, nb_profiles = valid buffer | RETURN_ERR | Should be Fail |
*/

void test_l1_lpa_hal_negative1_cellular_esim_get_profile_info(void)
{
    UT_LOG("Entering test_l1_lpa_hal_negative1_cellular_esim_get_profile_info ...");
    int nb_profiles = 0;
    eSIMProfileStruct **profile_list =NULL; 
    UT_LOG("Invoking cellular_esim_get_profile_info with NULL profile list parameters.");
    int result = cellular_esim_get_profile_info(&profile_list, &nb_profiles);
    UT_LOG("cellular_esim_get_profile_info Return result: %d", result);
    UT_ASSERT_EQUAL(result, RETURN_ERROR);
    UT_LOG("Exiting test_l1_lpa_hal_negative1_cellular_esim_get_profile_info ...");
}


/**
 * @brief Validates the handling of null pointers by the cellular_esim_get_profile_info function.
 *
 * This test case is designed to confirm that the cellular_esim_get_profile_info function gracefully handles null pointers. @n
 * @n
 * **Test Group ID:** Basic: 01 @n
 * **Test Case ID:** 011 @n
 * **Priority:** High @n
 * @n
 * **Pre-Conditions:** None @n
 * **Dependencies:** None @n
 * **User Interaction:** If user chose to run the test in interactive mode, then the test case has to be selected via console. @n
 *  @n
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data |Expected Result |Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Invoke cellular_esim_get_profile_info with NULL pointer as nb_profiles parameter | valid buffer, nb_profiles = NULL | RETURN_ERR | Should be Fail |
 */

void test_l1_lpa_hal_negative2_cellular_esim_get_profile_info(void)
{
    UT_LOG("Entering test_l1_lpa_hal_negative2_cellular_esim_get_profile_info ...");
    eSIMProfileStruct *profile_list = NULL;

    int *nb_profiles = NULL;
    UT_LOG("Invoking cellular_esim_get_profile_info with NULL nb_profiles parameter.");
    int result = cellular_esim_get_profile_info(&profile_list, nb_profiles);
    UT_LOG("cellular_esim_get_profile_info Return result: %d", result);
    UT_ASSERT_EQUAL(result, RETURN_ERROR);
    free(profile_list);
    //profile_list=NULL;

    UT_LOG("Exiting test_l1_lpa_hal_negative2_cellular_esim_get_profile_info ...");
}

 /**
* @brief Testing the functionality of 'cellular_esim_enable_profile' API
*
* This unit test is designed to evaluate the success case of the cellular_esim_enable_profile API function. @n
* @n
* **Test Group ID:** Basic: 01 @n
* **Test Case ID:** 012 @n
* **Priority:** High @n
*  @n
* **Pre-Conditions:** None @n
* **Dependencies:** None @n
* **User Interaction:** If user chose to run the test in interactive mode, then the test case has to be selected via console @n
*  @n
* **Test Procedure:**@n
* | Variation / Step | Description | Test Data |Expected Result |Notes |
* | :----: | --------- | ---------- |-------------- | ----- |
* | 01 | The function 'cellular_esim_enable_profile' is invoked with valid 'iccid' and 'iccid_size' | iccid = valid, iccid_size = 20 | RETURN_OK | Should be successful |
*/
void test_l1_lpa_hal_positive1_cellular_esim_enable_profile( void )
{
    UT_LOG("Entering test_l1_lpa_hal_positive1_cellular_esim_enable_profile...");
    int iccid_size =20;
    int ret_value = 0;
    for (int i = 0;i < num_iccid; i++)
    {
        UT_LOG("Invoking cellular_esim_enable_profile with valid iccid : %s and iccid_size : %d.",iccid[i],iccid_size);
        ret_value = cellular_esim_enable_profile(iccid[i], iccid_size);
        UT_LOG("cellular_esim_enable_profile Return ret_value : %d", ret_value);
        UT_ASSERT_EQUAL(ret_value, RETURN_OK);
    }
    UT_LOG("Exiting test_l1_lpa_hal_positive1_cellular_esim_enable_profile...");
}

/**
* @brief Test to check the handling of NULL iccid input case in the cellular_esim_enable_profile API function
*
* The function is designed to test the error behavior API when provided with a NULL iccid input. @n
* @n
* **Test Group ID:** Basic: 01 @n
* **Test Case ID:** 013 @n
* **Priority:** High @n
*  @n
* **Pre-Conditions:** None @n
* **Dependencies:** None @n
* **User Interaction:** If user chose to run the test in interactive mode, then the test case has to be selected via console @n
*  @n
* **Test Procedure:**@n
* | Variation / Step | Description | Test Data |Expected Result |Notes |
* | ---- | --------- | ---------- |-------------- | ----- |
* | 01 | Invoke the API cellular_esim_enable_profile API with NULL iccid | iccid = NULL, iccid_size = 20 | RETURN_ERR | Should be Fail |
*/

void test_l1_lpa_hal_negative1_cellular_esim_enable_profile( void )
{
    UT_LOG("Entering test_l1_lpa_hal_negative1_cellular_esim_enable_profile...");
    char *iccid = NULL; 
    int iccid_size = 20;
    int ret_value = 0;
    UT_LOG("Invoking cellular_esim_enable_profile with NULL iccid ");
    ret_value = cellular_esim_enable_profile(iccid, iccid_size);
    UT_LOG("cellular_esim_enable_profile Return ret_value: %d", ret_value);
    UT_ASSERT_EQUAL(ret_value, RETURN_ERROR);
    UT_LOG("Exiting test_l1_lpa_hal_negative1_cellular_esim_enable_profile...");
}

/**
 * @brief Test function for negative scenarios in cellular_esim_enable_profile API.
 *
 * This test is focused on validating the cellular_esim_enable_profile API response when iccid is Invalid. @n
 * @n
 * **Test Group ID:** Basic: 01 @n
 * **Test Case ID:** 014 @n
 * **Priority:** High @n
 *  @n
 * **Pre-Conditions:** None @n
 * **Dependencies:** None @n
 * **User Interaction:** If user chose to run the test in interactive mode, then the test case has to be selected via console. @n
 *  @n
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data |Expected Result |Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Invoke cellular_esim_enable_profile API with a Invalid value for iccid | iccid = "98414102915071@#0054", iccid_size = 20 | RETURN_ERR | Should fail |
 */
void test_l1_lpa_hal_negative2_cellular_esim_enable_profile( void )
{
    UT_LOG("Entering test_l1_lpa_hal_negative2_cellular_esim_enable_profile...");
    char *iccid = "98414102915071@#0054";
    int iccid_size = 20;
    int ret_value = 0;
    UT_LOG("Invoking cellular_esim_enable_profile with alphanumeric value : %s",iccid);
    ret_value = cellular_esim_enable_profile(iccid, iccid_size);
    UT_LOG("cellular_esim_enable_profile Return ret_value: %d", ret_value);
    UT_ASSERT_EQUAL(ret_value, RETURN_ERROR);
    UT_LOG("Exiting test_l1_lpa_hal_negative2_cellular_esim_enable_profile...");
}
/**
 * @brief Test function for cellular_esim_enable_profile API.
 *
 * This test is focused on validating the cellular_esim_enable_profile API response when iccid is empty string. @n
 * @n
 * **Test Group ID:** Basic: 01 @n
 * **Test Case ID:** 015 @n
 * **Priority:** High @n
 *  @n
 * **Pre-Conditions:** None @n
 * **Dependencies:** None @n
 * **User Interaction:** If user chose to run the test in interactive mode, then the test case has to be selected via console. @n
 *  @n
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data |Expected Result |Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Invoke cellular_esim_enable_profile API with empty string for iccid | iccid = "", iccid_size = 20 | RETURN_ERR | Should fail |
 */
void test_l1_lpa_hal_negative3_cellular_esim_enable_profile( void )
{
    UT_LOG("Entering test_l1_lpa_hal_negative3_cellular_esim_enable_profile...");
    char *iccid = "";
    int iccid_size = 20;
    int ret_value = 0;
    UT_LOG("Invoking cellular_esim_enable_profile with Empty string ");
    ret_value = cellular_esim_enable_profile(iccid, iccid_size);
    UT_LOG("cellular_esim_enable_profile Return ret_value: %d", ret_value);
    UT_ASSERT_EQUAL(ret_value, RETURN_ERROR);
    UT_LOG("Exiting test_l1_lpa_hal_negative3_cellular_esim_enable_profile...");
}
/**
 * @brief Test function for negative scenarios in cellular_esim_enable_profile API.
 *
 * This test is focused on validating the cellular_esim_enable_profile API response when iccid is invalid. @n
 * @n
 * **Test Group ID:** Basic: 01 @n
 * **Test Case ID:** 016 @n
 * **Priority:** High @n
 *  @n
 * **Pre-Conditions:** None @n
 * **Dependencies:** None @n
 * **User Interaction:** If user chose to run the test in interactive mode, then the test case has to be selected via console. @n
 *  @n
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data |Expected Result |Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Invoke cellular_esim_enable_profile API with Invalid value | iccid = "random", iccid_size = 20 | RETURN_ERR | Should fail |
 */
void test_l1_lpa_hal_negative4_cellular_esim_enable_profile( void )
{
    UT_LOG("Entering test_l1_lpa_hal_negative4_cellular_esim_enable_profile...");
    char *iccid = "random";
    int iccid_size = 20;
    int ret_value = 0;
    UT_LOG("Invoking cellular_esim_enable_profile with Invalid iccid : %s",iccid);
    ret_value = cellular_esim_enable_profile(iccid, iccid_size);
    UT_LOG("cellular_esim_enable_profile Return ret_value: %d", ret_value);
    UT_ASSERT_EQUAL(ret_value, RETURN_ERROR);
    UT_LOG("Exiting test_l1_lpa_hal_negative4_cellular_esim_enable_profile...");
}

/**
 * @brief Tests the positive scenario of the cellular_esim_disable_profile API
 *
 * This test is used to verify whether the cellular_esim_disable_profile API behaves expectedly. @n
 * @n
 * **Test Group ID:** Basic: 01 @n
 * **Test Case ID:** 017 @n
 * **Priority:** High @n
 *  @n
 * **Pre-Conditions:** None @n
 * **Dependencies:** None @n
 * **User Interaction:** If user chose to run the test in interactive mode, then the test case has to be selected via console @n
 *  @n
 * **Test Procedure:**@n
 * | Variation / Step | Description      | Test Data     | Expected Result | Notes     |
 * | :----:            | ---------       | ----------    |--------------   | -----     |
 * | 01                | Invoke cellular_esim_disable_profile() with valid iccid and iccid_size   | iccid = valid, iccid_size = 20  | RETURN_OK   | Should be successful |
 */
void test_l1_lpa_hal_positive1_cellular_esim_disable_profile(void) 
{
    UT_LOG("Entering test_l1_lpa_hal_positive1_cellular_esim_disable_profile...");
    int iccid_size = 20;
    for (int i = 0;i < num_iccid; i++)
    {
        UT_LOG("Invoking cellular_esim_disable_profile() with  valid iccid: %s and iccid_size: %d",iccid[i],iccid_size); 
        int result = cellular_esim_disable_profile(iccid[i], iccid_size);
        UT_LOG("cellular_esim_disable_profile Return result: %d", result);
        UT_ASSERT_EQUAL(result, RETURN_OK);
    }
    UT_LOG("Exiting test_l1_lpa_hal_positive1_cellular_esim_disable_profile...");
}

/**
* @brief This test functions checks the negative behaviour of the function cellular_esim_disable_profile
*
* This test stimulates the scenario where the function cellular_esim_disable_profile is invoked with NULL iccid and an integer size of 20.  @n
* @n
* **Test Group ID:** Basic: 01 @n
* **Test Case ID:** 018 @n
* **Priority:** High @n
*  @n
* **Pre-Conditions:** None @n
* **Dependencies:** None @n
* **User Interaction:** If user chose to run the test in interactive mode, then the test case has to be selected via console @n
*  @n
* **Test Procedure:**@n
* | Variation / Step | Description | Test Data |Expected Result |Notes |
* | :----: | --------- | ---------- |-------------- | ----- |
* | 01 | Invoke cellular_esim_disable_profile() with NULL iccid and iccid_size equals to 20 | iccid = NULL, iccid_size = 20 | RETURN_ERR | Should be Fail |
*/

void test_l1_lpa_hal_negative1_cellular_esim_disable_profile(void) 
{
    UT_LOG("Entering test_l1_lpa_hal_negative1_cellular_esim_disable_profile...");
    char *iccid = NULL;
    int iccid_size = 20;
    UT_LOG("Invoking cellular_esim_disable_profile() with NULL iccid ");
    int result = cellular_esim_disable_profile(iccid, iccid_size);
    UT_LOG("cellular_esim_disable_profile Return result: %d", result);
    UT_ASSERT_EQUAL(result, RETURN_ERROR);
    UT_LOG("Exiting test_l1_lpa_hal_negative1_cellular_esim_disable_profile...");
}

/**
* @brief This test case investigates the function cellular_esim_disable_profile() with an iccid containing alphanumeric or special characters.
*
* In this test, the function cellular_esim_disable_profile() is invoked with a iccid that includes alphanumeric or special characters and a iccid_size equals to 20. @n
* @n
* **Test Group ID:** Basic: 01 @n
* **Test Case ID:** 019 @n
* **Priority:** High @n
* @n
* **Pre-Conditions:** None @n
* **Dependencies:** None @n
* **User Interaction:** If user chose to run the test in interactive mode, then the test case has to be selected via console. @n
*  @n
* **Test Procedure:**@n
* | Variation / Step | Description | Test Data |Expected Result |Notes |
* | :----: | --------- | ---------- |-------------- | ----- |
* | 01 | Invoking cellular_esim_disable_profile() with ICCID containing alphanumeric and special characters | iccid = "98410A00@04860024951", iccid_size = 20 | RETURN_ERR | Should be Fail |
*/
void test_l1_lpa_hal_negative2_cellular_esim_disable_profile(void) 
{
    UT_LOG("Entering test_l1_lpa_hal_negative2_cellular_esim_disable_profile...");
    char *iccid = "98410A00@04860024951";
    int iccid_size = 20;
    UT_LOG("Invoking cellular_esim_disable_profile() with iccid contains alphanumeric or special characters : %s",iccid);
    int result = cellular_esim_disable_profile(iccid, iccid_size);
    UT_LOG("cellular_esim_disable_profile Return result: %d", result);
    UT_ASSERT_EQUAL(result, RETURN_ERROR);
    UT_LOG("Exiting test_l1_lpa_hal_negative2_cellular_esim_disable_profile..."); 
}

/**
 * @brief This unit test is to check the behavior of the API 'cellular_esim_disable_profile' when invokded with empty ICCID
 *
 * In this test, the API cellular_esim_disable_profile is invoked with invalid ICCID and ICCID size equal to 20. @n
 * @n
 * **Test Group ID:** Basic: 01 @n
 * **Test Case ID:** 020 @n
 * **Priority:** High @n
 * @n
 * **Pre-Conditions:** None @n
 * **Dependencies:** None @n
 * **User Interaction:** If user chose to run the test in interactive mode, then the test case has to be selected via console. @n
 * @n
 * **Test Procedure:** @n
 * | Variation / Step | Description | Test Data |Expected Result |Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Invoke cellular_esim_disable_profile() with Invalid iccid string and iccid_size equals to 20  | iccid = invalid value, iccid_size = 20 | RETURN_ERR |  should be fail |
 */
void test_l1_lpa_hal_negative3_cellular_esim_disable_profile(void) 
{
    UT_LOG("Entering test_l1_lpa_hal_negative3_cellular_esim_disable_profile...");
    char *iccid = "984141";
    int iccid_size = 20;
    UT_LOG("Invoking cellular_esim_disable_profile() with Invalid iccid string: %s",iccid);
    int result = cellular_esim_disable_profile(iccid, iccid_size);
    UT_LOG("cellular_esim_disable_profile Return result: %d", result);
    UT_ASSERT_EQUAL(result, RETURN_ERROR);
    UT_LOG("Exiting test_l1_lpa_hal_negative3_cellular_esim_disable_profile...");
}

/**
* @brief Test for the cellular_esim_disable_profile API .
*
* This test validates the cellular_esim_disable_profile() function by testing if it can handle Empty string iccid and valid iccid_size . @n
* @n
* **Test Group ID:** Basic: 01 @n
* **Test Case ID:** 021 @n
* **Priority:** High @n
* @n
* **Pre-Conditions:** None @n
* **Dependencies:** None @n
* **User Interaction:** If user chose to run the test in interactive mode, then the test case has to be selected via console. @n
* @n
* **Test Procedure:**@n
* | Variation / Step | Description | Test Data |Expected Result |Notes |
* | :----: | --------- | ---------- |-------------- | ----- |
* | 01 | Invoke cellular_esim_disable_profile() with Empty iccid and iccid_size equals to 20. | iccid = "", iccid_size = 20 | RETURN_ERR | Should be Fail |
*/

void test_l1_lpa_hal_negative4_cellular_esim_disable_profile(void) 
{
    UT_LOG("Entering test_l1_lpa_hal_negative4_cellular_esim_disable_profile...");
    char *iccid = "";
    int iccid_size = 20;
    UT_LOG("Invoking cellular_esim_disable_profile() with Empty iccid ");
    int result = cellular_esim_disable_profile(iccid, iccid_size);
    UT_LOG("cellular_esim_disable_profile Return result: %d", result);
    UT_ASSERT_EQUAL(result, RETURN_ERROR);
    UT_LOG("Exiting test_l1_lpa_hal_negative4_cellular_esim_disable_profile...");
}

/**
* @brief Tests eSIM profile deletion on a cellular device
*
* This test case validates the eSIM profile deletion on a cellular device. @n
* @n
* **Test Group ID:** Basic: 01 @n
* **Test Case ID:** 022 @n
* **Priority:** High @n
* @n
* **Pre-Conditions:** None @n
* **Dependencies:** None @n
* **User Interaction:** If user chose to run the test in interactive mode, then the test case has to be selected via console. @n
*  @n
* **Test Procedure:**@n
* | Variation / Step | Description | Test Data |Expected Result |Notes |
* | :----: | --------- | ---------- |-------------- | ----- |
* | 01 | Invoking API cellular_esim_delete_profile with valid iccid value and iccid_size | iccid = valid, iccid_size = 20 | RETURN_OK | should be successful |
*/
void test_l1_lpa_hal_positive1_cellular_esim_delete_profile(void) 
{
    UT_LOG("Entering test_l1_lpa_hal_positive1_cellular_esim_delete_profile...");
    int iccid_size = 20;
    for (int i = 0;i < num_iccid; i++)
    {
        UT_LOG("Invoking cellular_esim_delete_profile with a valid ICCID : %s and iccid_size : %d",iccid[i],iccid_size);
        int status = cellular_esim_delete_profile(iccid[i], iccid_size);
        UT_LOG("cellular_esim_delete_profile Return status: %d", status);
        UT_ASSERT_EQUAL(status, RETURN_OK);
    }
    UT_LOG("Exiting test_l1_lpa_hal_positive1_cellular_esim_delete_profile...");
}

/**
 * @brief Tests negative scenarios for cellular_esim_delete_profile function
 *
 * This test checks whether the cellular_esim_delete_profile function is able to handle NULL ICCID argument, which is an invalid input. @n 
 * @n
 * **Test Group ID:** Basic: 01 @n
 * **Test Case ID:** 023 @n
 * **Priority:** High @n
 * @n
 * **Pre-Conditions:** None @n
 * **Dependencies:** None @n
 * **User Interaction:** If user chose to run the test in interactive mode, then the test case has to be selected via console
 * @n
 * **Test Procedure:**@n
 * | Variation / Step | Description   | Test Data     | Expected Result   | Notes |
 * | :-------------:  | ---------     | ----------    | --------------    | ----- |
 * | 01 |Invoke cellular_esim_delete_profile API with NULL iccid value | iccid = NULL, iccid_size = 20 | RETURN_ERR | Should fail |
 */
void test_l1_lpa_hal_negative1_cellular_esim_delete_profile(void)
{
    UT_LOG("Entering test_l1_lpa_hal_negative1_cellular_esim_delete_profile...");
    char *iccid = NULL;
    int iccid_size = 20;
    UT_LOG("Invoking cellular_esim_delete_profile with NULL ICCID. ");
    int status = cellular_esim_delete_profile(iccid, iccid_size);
    UT_LOG("cellular_esim_delete_profile Return status: %d", status);
    UT_ASSERT_EQUAL(status, RETURN_ERROR);
    UT_LOG("Exiting test_l1_lpa_hal_negative1_cellular_esim_delete_profile...");
}

/**
 * @brief Tests negative scenarios for cellular_esim_delete_profile function
 *
 * This test checks whether the cellular_esim_delete_profile function is able to handle invalid ICCID argument input.  @n
 * @n
 * **Test Group ID:** Basic: 01 @n
 * **Test Case ID:** 024 @n
 * **Priority:** High @n
 *  @n
 * **Pre-Conditions:** None @n
 * **Dependencies:** None @n
 * **User Interaction:** If user chose to run the test in interactive mode, then the test case has to be selected via console. @n
 *  @n
 * **Test Procedure:**@n
 * | Variation / Step | Description   | Test Data      | Expected Result      | Notes |
 * | :-------------:  | ---------     | ----------     | --------------       | ----- |
 * | 01 |Invoke cellular_esim_delete_profile API with invalid alphanumeric iccid value | iccid = 98109909002@43658739, iccid_size = 20 | RETURN_ERR | Should fail |
 */
void test_l1_lpa_hal_negative2_cellular_esim_delete_profile(void)
{
    UT_LOG("Entering test_l1_lpa_hal_negative2_cellular_esim_delete_profile...");
    char *iccid = "98109909002@43658739";
    int iccid_size = 20;
    UT_LOG("Invoking cellular_esim_delete_profile with Invalid ICCID : %s",iccid);
    int status = cellular_esim_delete_profile(iccid, iccid_size);
    UT_LOG("cellular_esim_delete_profile Return status: %d", status);
    UT_ASSERT_EQUAL(status, RETURN_ERROR);
    UT_LOG("Exiting test_l1_lpa_hal_negative2_cellular_esim_delete_profile...");
}
/**
 * @brief Tests negative scenarios for cellular_esim_delete_profile function
 *
 * This test checks whether the cellular_esim_delete_profile function is able to handle ICCID argument, which is an invalid input.  @n
 * @n
 * **Test Group ID:** Basic: 01 @n
 * **Test Case ID:** 025 @n
 * **Priority:** High @n
 *  @n
 * **Pre-Conditions:** None @n
 * **Dependencies:** None @n
 * **User Interaction:** If user chose to run the test in interactive mode, then the test case has to be selected via console. @n
 *  @n
 * **Test Procedure:**@n
 * | Variation / Step | Description   | Test Data    | Expected Result     | Notes |
 * | :-------------:  | ---------     | ----------   | --------------      | ----- |
 * |     01           |Invoke cellular_esim_delete_profile API with Empty string ICCID value | iccid = "", iccid_size = 20  | RETURN_ERR | Should be Fail |
 */
void test_l1_lpa_hal_negative3_cellular_esim_delete_profile(void)
{
    UT_LOG("Entering test_l1_lpa_hal_negative3_cellular_esim_delete_profile...");
    char *iccid = "";
    int iccid_size = 20;
    UT_LOG("Invoking cellular_esim_delete_profile with Empty string of ICCID ");
    int status = cellular_esim_delete_profile(iccid, iccid_size);
    UT_LOG("cellular_esim_delete_profile Return status: %d", status);
    UT_ASSERT_EQUAL(status, RETURN_ERROR);
    UT_LOG("Exiting test_l1_lpa_hal_negative3_cellular_esim_delete_profile...");
}
/**
 * @brief Tests negative scenarios for cellular_esim_delete_profile function.
 *
 * This test checks whether the cellular_esim_delete_profile function is able to handle ICCID argument, which is an invalid input.  @n
 * @n
 * **Test Group ID:** Basic: 01 @n
 * **Test Case ID:** 026 @n
 * **Priority:** High @n
 *  @n
 * **Pre-Conditions:** None @n
 * **Dependencies:** None @n
 * **User Interaction:** If user chose to run the test in interactive mode, then the test case has to be selected via console. @n
 *  @n
 * **Test Procedure:**@n
 * | Variation / Step | Description   | Test Data        | Expected Result     | Notes |
 * | :-------------:  | ---------     | ----------       | --------------      | ----- |
 * |     01   |Invoke cellular_esim_delete_profile API with Invalid ICCID value | iccid = random, iccid_size = 20  | RETURN_ERR | Should be Fail |
 */
void test_l1_lpa_hal_negative4_cellular_esim_delete_profile(void)
{
    UT_LOG("Entering test_l1_lpa_hal_negative4_cellular_esim_delete_profile...");
    char *iccid = "random";
    int iccid_size = 20;
    UT_LOG("Invoking cellular_esim_delete_profile with Invalid ICCID : %s",iccid);
    int status = cellular_esim_delete_profile(iccid, iccid_size);
    UT_LOG("cellular_esim_delete_profile Return status: %d", status);
    UT_ASSERT_EQUAL(status, RETURN_ERROR);
    UT_LOG("Exiting test_l1_lpa_hal_negative4_cellular_esim_delete_profile...");
}

/**
* @brief This is a unit test for the cellular_esim_lpa_init function call
*
* This test checks the operation of the cellular_esim_lpa_init function when provided with valid inputs. @n
* @n
* **Test Group ID:** Basic: 01 @n
* **Test Case ID:** 027 @n
* **Priority:** High @n
*  @n
* **Pre-Conditions:** None @n
* **Dependencies:** None @n
* **User Interaction:**: If user chose to run the test in interactive mode, then the test case has to be selected via console. @n
*  @n
* **Test Procedure:**@n
* | Variation / Step | Description | Test Data |Expected Result | Notes |
* | :----: | --------- | ---------- |-------------- | ----- |
* | 01 | Invoking the cellular_esim_lpa_init function | None | RETURN_OK | Should be Successful |
*/
void test_l1_lpa_hal_positive1_cellular_esim_lpa_init(void)
{
    UT_LOG("Entering test_l1_lpa_hal_positive1_cellular_esim_lpa_init...");
    UT_LOG("Invoking cellular_esim_lpa_init with No inputs required ");
    int res=cellular_esim_lpa_exit();
    UT_LOG("cellular_esim_lpa_exit Return res: %d", res);
    if(res!=0)
    {
        UT_LOG("LPA is not initialized");
        UT_FAIL("LPA uninitiaization failed");
    }
    int status = cellular_esim_lpa_init();
    UT_LOG("cellular_esim_lpa_init Return status: %d", status);

    UT_ASSERT_EQUAL(status, RETURN_OK);
    UT_LOG("Exiting test_l1_lpa_hal_positive1_cellular_esim_lpa_init...");
}

/**
* @brief Testing the successful exit of cellular_esim_lpa
*
* This test verifies the successful operation of the cellular_esim_lpa_exit function under normal operation. @n
* @n
* **Test Group ID:** Basic: 01 @n
* **Test Case ID:** 028 @n
* **Priority:** High @n
*  @n
* **Pre-Conditions:** None @n
* **Dependencies:** None @n
* **User Interaction:** If user chose to run the test in interactive mode, then the test case has to be selected via console @n
*  @n
* **Test Procedure:**@n
* | Variation / Step | Description | Test Data |Expected Result |Notes |
* | :----: | --------- | ---------- |-------------- | ----- |
* | 01 | Invoke the cellular_esim_lpa_exit() function | None | RETURN_OK | Should be Successful |
*/
void test_l1_lpa_hal_positive1_cellular_esim_exit(void)
{
    UT_LOG("Entering test_l1_lpa_hal_positive1_cellular_esim_lpa_exit()...");
    int returnValue = 0;
    UT_LOG("Invoking cellular_esim_lpa_exit() function");
    returnValue = cellular_esim_lpa_exit();
    UT_LOG("cellular_esim_lpa_exit Return returnValue: %d", returnValue);
    int res=cellular_esim_lpa_init();
    UT_LOG("cellular_esim_lpa_init Return res: %d", res);
    if(res!=0)
    {
        UT_LOG("LPA is not initialized");
        UT_FAIL("LPA initiaization failed");
    }
    UT_ASSERT_EQUAL(returnValue, RETURN_OK);
    UT_LOG("Exiting test_l1_lpa_hal_positive1_cellular_esim_lpa_exit()...");
}
/**
* @brief Test for verifying cellular_esim_get_eid function
*
* This unit test checks whether the cellular_esim_get_eid function retrieves the Embedded Identifier (EID) successfully for a cellular eSIM.
* @n
* **Test Group ID:** Basic: 01 @n
* **Test Case ID:** 029 @n
* **Priority:** High @n
* @n
* **Pre-Conditions:** None @n
* **Dependencies:** None @n
* **User Interaction:** If user chose to run the test in interactive mode, then the test case has to be selected via console @n
*  @n
* **Test Procedure:**@n
* | Variation / Step | Description | Test Data |Expected Result |Notes |
* | :----: | --------- | ---------- |-------------- | ----- |
* | 01 | Invoke cellular_esim_get_eid and verify the return value | None | RETURN_OK | Should be Successful |
*/

void test_l1_lpa_hal_positive1_cellular_esim_get_eid(void)
{   
    UT_LOG("Entering test_l1_lpa_hal_positive1_cellular_esim_get_eid...");
    int returnValue = 0;
    UT_LOG("Invoking cellular_esim_get_eid() function");
    returnValue = cellular_esim_get_eid();
    UT_LOG("cellular_esim_get_eid Return returnValue: %d", returnValue);

    UT_ASSERT_EQUAL(returnValue, RETURN_OK);

    UT_LOG("Exiting test_l1_lpa_hal_positive1_cellular_esim_get_eid...");
}

/**
* @brief Function to test cellular_esim_get_euicc API in L1 LPA HAL
*
* This function tests the cellular_esim_get_euicc API in the L1 Layer . @n 
* @n
* **Test Group ID:** Basic: 01 @n
* **Test Case ID:** 030 @n
* **Priority:** High @n
* @n
* **Pre-Conditions:** None @n
* **Dependencies:** None @n
* **User Interaction:** If user chose to run the test in interactive mode, then the test case has to be selected via console @n
* @n
* **Test Procedure:**@n
* | Variation / Step | Description | Test Data |Expected Result |Notes |
* | :----: | --------- | ---------- |-------------- | ----- |
* | 01 | Invokes cellular_esim_get_euicc() and verify the return value | None | RETURN_OK | Should be successful |
*
*/
void test_l1_lpa_hal_positive1_cellular_esim_get_euicc(void)
{
    UT_LOG("Entering test_l1_lpa_hal_positive1_cellular_esim_get_euicc...");
    int returnValue = 0; 
    UT_LOG("Invoking cellular_esim_get_euicc with no input parameters.");
    
    returnValue = cellular_esim_get_euicc();
    UT_LOG("cellular_esim_get_euicc Return returnValue: %d", returnValue);

    UT_ASSERT_EQUAL(returnValue, RETURN_OK);
    UT_LOG("Exiting test_l1_lpa_hal_positive1_cellular_esim_get_euicc...");
}

int init_cellular_esim_init(){
    int ret = 0;
    ret = cellular_esim_lpa_init();
    if (ret == 0)
    {
        UT_LOG("celular_esim init returned success");
    }
    else
    {
        UT_LOG("celular_esim  init returned failure");
        UT_FAIL_FATAL("celular_esim  initialization failed");
    }
    return 0;
}
int clean_cellular_esim_exit(){
    int ret = 0;
    ret = cellular_esim_lpa_exit();
    if (ret == 0)
    {
        UT_LOG("celular_esim exit returned success");
    }
    else
    {
        UT_LOG("celular_esim exit returned failure");
        UT_FAIL_FATAL("celular_esim exit failed");
    }
    return 0;
}
static UT_test_suite_t * pSuite = NULL;

/**
 * @brief Register the main tests for this module
 *
 * @return int - 0 on success, otherwise failure
 */
int test_lpa_hal_l1_register(void)
{
    // Create the test suite
    pSuite = UT_add_suite("[L1 lpa_hal]",init_cellular_esim_init,clean_cellular_esim_exit);
    if (pSuite == NULL) {
        return -1;
    }
    UT_add_test( pSuite, "l1_lpa_hal_positive1_cellular_esim_download_profile_from_smds", test_l1_lpa_hal_positive1_cellular_esim_download_profile_from_smds);
    UT_add_test( pSuite, "l1_lpa_hal_negative1_cellular_esim_download_profile_from_smds", test_l1_lpa_hal_negative1_cellular_esim_download_profile_from_smds);
    UT_add_test( pSuite, "l1_lpa_hal_negative2_cellular_esim_download_profile_from_smds", test_l1_lpa_hal_negative2_cellular_esim_download_profile_from_smds);
    UT_add_test( pSuite, "l1_lpa_hal_negative3_cellular_esim_download_profile_from_smds", test_l1_lpa_hal_negative3_cellular_esim_download_profile_from_smds);
    UT_add_test( pSuite, "l1_lpa_hal_positive1_cellular_esim_download_profile_from_defaultsmdp", test_l1_lpa_hal_positive1_cellular_esim_download_profile_from_defaultsmdp);
    UT_add_test( pSuite, "l1_lpa_hal_negative1_cellular_esim_download_profile_from_defaultsmdp", test_l1_lpa_hal_negative1_cellular_esim_download_profile_from_defaultsmdp);
    UT_add_test( pSuite, "l1_lpa_hal_negative2_cellular_esim_download_profile_from_defaultsmdp", test_l1_lpa_hal_negative2_cellular_esim_download_profile_from_defaultsmdp);
    UT_add_test( pSuite, "l1_lpa_hal_negative3_cellular_esim_download_profile_from_defaultsmdp", test_l1_lpa_hal_negative3_cellular_esim_download_profile_from_defaultsmdp);
    UT_add_test( pSuite, "l1_lpa_hal_positive1_cellular_esim_get_profile_info", test_l1_lpa_hal_positive1_cellular_esim_get_profile_info);
    UT_add_test( pSuite, "l1_lpa_hal_negative1_cellular_esim_get_profile_info", test_l1_lpa_hal_negative1_cellular_esim_get_profile_info);
    UT_add_test( pSuite, "l1_lpa_hal_negative2_cellular_esim_get_profile_info", test_l1_lpa_hal_negative2_cellular_esim_get_profile_info);
    UT_add_test( pSuite, "l1_lpa_hal_positive1_cellular_esim_enable_profile", test_l1_lpa_hal_positive1_cellular_esim_enable_profile);
    UT_add_test( pSuite, "l1_lpa_hal_negative1_cellular_esim_enable_profile", test_l1_lpa_hal_negative1_cellular_esim_enable_profile);
    UT_add_test( pSuite, "l1_lpa_hal_negative2_cellular_esim_enable_profile", test_l1_lpa_hal_negative2_cellular_esim_enable_profile);
    UT_add_test( pSuite, "l1_lpa_hal_negative3_cellular_esim_enable_profile", test_l1_lpa_hal_negative3_cellular_esim_enable_profile);
    UT_add_test( pSuite, "l1_lpa_hal_negative4_cellular_esim_enable_profile", test_l1_lpa_hal_negative4_cellular_esim_enable_profile);
    UT_add_test( pSuite, "l1_lpa_hal_positive1_cellular_esim_disable_profile", test_l1_lpa_hal_positive1_cellular_esim_disable_profile);
    UT_add_test( pSuite, "l1_lpa_hal_negative1_cellular_esim_disable_profile", test_l1_lpa_hal_negative1_cellular_esim_disable_profile);
    UT_add_test( pSuite, "l1_lpa_hal_negative2_cellular_esim_disable_profile", test_l1_lpa_hal_negative2_cellular_esim_disable_profile);
    UT_add_test( pSuite, "l1_lpa_hal_negative3_cellular_esim_disable_profile", test_l1_lpa_hal_negative3_cellular_esim_disable_profile);
    UT_add_test( pSuite, "l1_lpa_hal_negative4_cellular_esim_disable_profile", test_l1_lpa_hal_negative4_cellular_esim_disable_profile);
    UT_add_test( pSuite, "l1_lpa_hal_positive1_cellular_esim_delete_profile", test_l1_lpa_hal_positive1_cellular_esim_delete_profile);
    UT_add_test( pSuite, "l1_lpa_hal_negative1_cellular_esim_delete_profile", test_l1_lpa_hal_negative1_cellular_esim_delete_profile);
    UT_add_test( pSuite, "l1_lpa_hal_negative2_cellular_esim_delete_profile", test_l1_lpa_hal_negative2_cellular_esim_delete_profile);
    UT_add_test( pSuite, "l1_lpa_hal_negative3_cellular_esim_delete_profile", test_l1_lpa_hal_negative3_cellular_esim_delete_profile);
    UT_add_test( pSuite, "l1_lpa_hal_negative4_cellular_esim_delete_profile", test_l1_lpa_hal_negative4_cellular_esim_delete_profile);
    UT_add_test( pSuite, "l1_lpa_hal_positive1_cellular_esim_lpa_init", test_l1_lpa_hal_positive1_cellular_esim_lpa_init);
    UT_add_test( pSuite, "l1_lpa_hal_positive1_cellular_esim_exit", test_l1_lpa_hal_positive1_cellular_esim_exit);
    UT_add_test( pSuite, "l1_lpa_hal_positive1_cellular_esim_get_eid", test_l1_lpa_hal_positive1_cellular_esim_get_eid);
    UT_add_test( pSuite, "l1_lpa_hal_positive1_cellular_esim_get_euicc", test_l1_lpa_hal_positive1_cellular_esim_get_euicc);
    return 0;
}

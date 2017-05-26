#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "main.h"
#include "utils.h"
#include "bruteforce.h"
#include "sha256.h"

const char* charset = "abcdefghijklmnopqrstuvwxyz"; //ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";


/*  check_hash_sha256 generates a hash of a given cleartext and 
 *  compares the result with a target hash. The function returns
 *  the cleartext if its hashed version is the target hash 
 */

char* check_hash_sha256(char* cleartext, char* target)
{
    SHA256_CTX ctx;

    int n = strlen(cleartext/*, BUFFER_SIZE*/);
    uint8_t digest_cleartext[SHA256_DIGEST_LENGTH];

    unsigned char hash[SHA256_LENGTH+1];

    sha256_init(&ctx);
    sha256_update(&ctx, (unsigned char*)cleartext, strlen(cleartext));
    sha256_final(&ctx, digest_cleartext);

    for (n=0; n < SHA256_DIGEST_LENGTH; n++)
        sprintf((char*)(hash + n*2), "%02x", digest_cleartext[n]);
    hash[SHA256_LENGTH] = '\0';

    if(strncmp((const char*)hash, target, SHA256_LENGTH) == 0)
        return cleartext;

    return NULL;
}


/*  This function bruteforces the key space from start
*   to end by scrolling through indices in an array
*   which correspond to the character representation of
*   the key in order to acurately replace the correct 
*   character in the key string with O(1). 
*/

char* brute(char* target, const char* start, const char* end)
{
    // Result is NULL if we cant find the matching string
    char* result = NULL;

    // Save the index of the last element in our alphabet
    int charset_last = strlen(charset/*, BUFFER_SIZE*/) -1;

    // Tracks our carry position
    int c;

    // Track the overall string length
    int base = (int)strlen(start/*, max_password_length*/); 
    int limit = (int)strlen(end/*, max_password_length*/); 

    // Create and initialize the array which saves
    // the generated keys as charset indices chain
    int* indices = gen_indices(start, base);

    // Allocate the string which holds the actual key
    char* current = malloc(sizeof(char) * (base+1));
    strncpy(current, start, base+1);

    // Increment the base if all passwords possible with
    // the given charset have been tried
    while(base <= limit)
    {
        // As long as the last permutation has not been hit
        while(indices[base-1] <= charset_last)
        {
            // Calculate hash of the current key and
            // compare to the given target hash 
            if((result = check_hash_sha256(current, target)) != NULL)
                break;

            // Because we want to have split key spaces we
            // need to check border case on each iteration
            if(strncmp(current, end, limit) == 0)
                break;

            // Find the position to increment
            for(c=0; c < base-1; c++)
            {
                // Do we have to carry over?
                if(indices[c] == charset_last)
                {
                    // All positions before carry have to be reset
                    indices[c] = 0;
                    current[c] = charset[0];
                    continue;
                }

                // Now we definately found the position because
                // we skipped all positions with last charset index
                break;
            }

            // Finally update indices and key to 
            // get the next element in charset
            indices[c]++;
            current[c] = charset[indices[c]];
        }

        // Break out this loop as well in case we got a result
        if(result)
            break;

        // There are no permutations left for the current length 
        // of the key buffer so we have to append another byte.
        base++;

        indices = expand_indices(indices, base);
        current = expand_buffer(current, base);

    }

    // Save our result
    if(result)
    {
        strncpy(target, current, base);
        target[base] = '\0';
    }
    else
    {
        target = NULL;
    }

    // Clean up
    free(indices);
    free(current);
    indices = NULL;
    current = NULL;

    return target;
}
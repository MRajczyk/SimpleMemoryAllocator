/*
 * Unit Test Bootstrap
 * Autor: Tomasz Jaworski, 2018-2020
 *
 * Test dla zadania Projekt: Rozszerzenie na ocenę 5,0
 * Autor testowanej odpowiedzi: Mikołaj Rajczyk
 * Test wygenerowano automatycznie o 2021-11-22 22:42:00.056458
 *
 * Debug: 
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <signal.h>
#include <setjmp.h>
#include <assert.h>

#if !defined(__clang__) && !defined(__GNUC__)
// Zakomentuj poniższy błąd, jeżeli chcesz przetestować testy na swoim kompilatorze C.
#error System testow jednostkowych jest przeznaczony dla kompilatorów GCC/Clang.
#endif

#if defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
// Zakomentuj poniższy błąd, jeżeli chcesz przetestować testy na platformie Windows.
#error System testow jednostkowych NIE jest przeznaczony dla testów uruchamianych na platformach Windows.
#endif

#define _RLDEBUG_API_
#include "unit_helper_v2.h"
#include "rdebug.h"

#include "tested_declarations.h"
#include "rdebug.h"

//
// Elementy globalne dla całego testu
//



            void *heap_malloc_test_thread(void *a)
            {

               char *ptr[2140];
               int ptr_state[2140] = {0};

               int is_allocated = 0;

               for (int i = 0; i < 2140; ++i)
               {
                 int rand_value = rand() % 100;
                 if (rand_value < 16)
                 {
                   for (int j = 0; j < 2140; ++j)
                     if (ptr_state[j] == 0)
                     {
                       ptr_state[j] = 1;
                       ptr[j] = heap_realloc(NULL, rand() % 100 + 50);  
                       test_error((intptr_t)ptr[j] % sizeof(void *) == 0, "Funkcja heap_realloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");
                       is_allocated++;
                       break;
                     }
                 }
                 else if (rand_value < 33)
                 {
                   if (is_allocated)
                   {
                      int to_reallocate = rand() % is_allocated;
                      for (int j = 0; j < 2140; ++j)
                       {
                           if (ptr_state[j] == 1 && !to_reallocate)
                           {
                               if (rand() % 100 < 50)
                                   ptr[j] = heap_realloc(ptr[j], rand() % 100 + 50);
                               else
                                   ptr[j] = heap_realloc(ptr[j], rand() % 1000 + 500);    
                               test_error((intptr_t)ptr[j] % sizeof(void *) == 0, "Funkcja heap_realloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");
                               break;
                           }
                           to_reallocate--;
                       }
                   }
                 }
                 else if (rand_value < 50)
                 {
                   if (is_allocated)
                   {
                     int to_free = rand() % is_allocated;
                      for (int j = 0; j < 2140; ++j)
                       {
                           if (ptr_state[j] == 1 && !to_free)
                           {
                             ptr_state[j] = 0;
                             is_allocated--;
                             heap_realloc(ptr[j], 0);
                             break;
                           }
                           to_free--;
                       }
                   }
                 }
                 else if (rand_value < 66)
                 {
                   for (int j = 0; j < 2140; ++j)
                     if (ptr_state[j] == 0)
                     {
                       ptr_state[j] = 1;
                       ptr[j] = heap_malloc(rand() % 100 + 50);
                       test_error((intptr_t)ptr[j] % sizeof(void *) == 0, "Funkcja heap_malloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");
                       is_allocated++;
                       break;
                     }
                 }
                 else if (rand_value < 83)
                 {
                   for (int j = 0; j < 2140; ++j)
                     if (ptr_state[j] == 0)
                     {
                       ptr_state[j] = 1;
                       ptr[j] = heap_calloc(rand() % 100 + 50, rand() % 10 + 1);  
                       test_error((intptr_t)ptr[j] % sizeof(void *) == 0, "Funkcja heap_calloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");
                       is_allocated++;
                       break;
                     }
                 }     
                 else
                 {
                   if (is_allocated)
                   {
                     int to_free = rand() % is_allocated;
                      for (int j = 0; j < 2140; ++j)
                       {
                           if (ptr_state[j] == 1 && !to_free)
                           {
                             ptr_state[j] = 0;
                             is_allocated--;
                             heap_free(ptr[j]);
                             break;
                           }
                           to_free--;
                       }
                   }
                 }
                 int status = heap_validate();
                 test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła %d", status);           
               }

               for (int j = 0; j < 2140; ++j)
                 if (ptr_state[j] == 1)
                     heap_realloc(ptr[j], 0);


            
            return a;
        }

            void *heap_alloc_aligned_test_thread(void *a)
            {

               char *ptr[2140];
               int ptr_state[2140] = {0};

               int is_allocated = 0;

               for (int i = 0; i < 2140; ++i)
               {
                 int rand_value = rand() % 100;
                 if (rand_value < 16)
                 {
                   for (int j = 0; j < 2140; ++j)
                     if (ptr_state[j] == 0)
                     {
                       ptr_state[j] = 1;
                       ptr[j] = heap_realloc_aligned(NULL, rand() % 100 + 50);  
                       test_error((intptr_t)ptr[j] % sizeof(void *) == 0, "Funkcja heap_realloc_aligned() powinna zwrócić adres będący wielokrotnością słowa maszynowego");
                       is_allocated++;
                       break;
                     }
                 }
                 else if (rand_value < 33)
                 {
                   if (is_allocated)
                   {
                      int to_reallocate = rand() % is_allocated;
                      for (int j = 0; j < 2140; ++j)
                       {
                           if (ptr_state[j] == 1 && !to_reallocate)
                           {
                               if (rand() % 100 < 50)
                                   ptr[j] = heap_realloc_aligned(ptr[j], rand() % 100 + 50);
                               else
                                   ptr[j] = heap_realloc_aligned(ptr[j], rand() % 1000 + 500);    
                               test_error((intptr_t)ptr[j] % sizeof(void *) == 0, "Funkcja heap_realloc_aligned() powinna zwrócić adres będący wielokrotnością słowa maszynowego");
                               break;
                           }
                           to_reallocate--;
                       }
                   }
                 }
                 else if (rand_value < 50)
                 {
                   if (is_allocated)
                   {
                     int to_free = rand() % is_allocated;
                      for (int j = 0; j < 2140; ++j)
                       {
                           if (ptr_state[j] == 1 && !to_free)
                           {
                             ptr_state[j] = 0;
                             is_allocated--;
                             heap_realloc_aligned(ptr[j], 0);
                             break;
                           }
                           to_free--;
                       }
                   }
                 }
                 else if (rand_value < 66)
                 {
                   for (int j = 0; j < 2140; ++j)
                     if (ptr_state[j] == 0)
                     {
                       ptr_state[j] = 1;
                       ptr[j] = heap_malloc_aligned(rand() % 100 + 50);
                       test_error((intptr_t)ptr[j] % sizeof(void *) == 0, "Funkcja heap_malloc_aligned() powinna zwrócić adres będący wielokrotnością słowa maszynowego");
                       is_allocated++;
                       break;
                     }
                 }
                 else if (rand_value < 83)
                 {
                   for (int j = 0; j < 2140; ++j)
                     if (ptr_state[j] == 0)
                     {
                       ptr_state[j] = 1;
                       ptr[j] = heap_calloc_aligned(rand() % 100 + 50, rand() % 10 + 1);  
                       test_error((intptr_t)ptr[j] % sizeof(void *) == 0, "Funkcja heap_calloc_aligned() powinna zwrócić adres będący wielokrotnością słowa maszynowego");
                       is_allocated++;
                       break;
                     }
                 }     
                 else
                 {
                   if (is_allocated)
                   {
                     int to_free = rand() % is_allocated;
                      for (int j = 0; j < 2140; ++j)
                       {
                           if (ptr_state[j] == 1 && !to_free)
                           {
                             ptr_state[j] = 0;
                             is_allocated--;
                             heap_free(ptr[j]);
                             break;
                           }
                           to_free--;
                       }
                   }
                 }
                 int status = heap_validate();
                 test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła %d", status);           
               }

               for (int j = 0; j < 2140; ++j)
                 if (ptr_state[j] == 1)
                     heap_realloc_aligned(ptr[j], 0);


            
            return a;
        }

            void *heap_alloc_ultimate_test_thread(void *a)
            {

               char *ptr[2140];
               int ptr_state[2140] = {0};

               int is_allocated = 0;

               for (int i = 0; i < 2140; ++i)
               {
                 int rand_value = rand() % 100;
                 if (rand_value < 10)
                 {
                   for (int j = 0; j < 2140; ++j)
                     if (ptr_state[j] == 0)
                     {
                       ptr_state[j] = 1;
                       ptr[j] = heap_realloc_aligned_debug(NULL, rand() % 1000 + 500, __LINE__, __FILE__);  
                       test_error(((intptr_t)ptr[j] & (intptr_t)(PAGE_SIZE - 1)) == 0, "Funkcja heap_realloc_aligned_debug() powinien zwrócić adres zaczynający się na początku strony");
                       is_allocated++;
                       break;
                     }
                 }
                 else if (rand_value < 20)
                 {
                   for (int j = 0; j < 2140; ++j)
                     if (ptr_state[j] == 0)
                     {
                       ptr_state[j] = 1;
                       ptr[j] = heap_calloc_aligned_debug(rand() % 1000 + 500, rand() % 4 + 1, __LINE__, __FILE__);  
                       test_error(((intptr_t)ptr[j] & (intptr_t)(PAGE_SIZE - 1)) == 0, "Funkcja heap_calloc_aligned_debug() powinien zwrócić adres zaczynający się na początku strony");
                       is_allocated++;
                       break;
                     }
                 }
                 else if (rand_value < 30)
                 {
                   for (int j = 0; j < 2140; ++j)
                     if (ptr_state[j] == 0)
                     {
                       ptr_state[j] = 1;
                       ptr[j] = heap_malloc_aligned_debug(rand() % 1000 + 500, __LINE__, __FILE__);  
                       test_error(((intptr_t)ptr[j] & (intptr_t)(PAGE_SIZE - 1)) == 0, "Funkcja heap_malloc_aligned_debug() powinien zwrócić adres zaczynający się na początku strony");
                       is_allocated++;
                       break;
                     }
                 }
                 else if (rand_value < 40)
                 {
                   for (int j = 0; j < 2140; ++j)
                     if (ptr_state[j] == 0)
                     {
                       ptr_state[j] = 1;
                       ptr[j] = heap_realloc_debug(NULL, rand() % 1000 + 500, __LINE__, __FILE__);  
                       test_error((intptr_t)ptr[j] % sizeof(void *) == 0, "Funkcja heap_realloc_debug() powinien zwrócić adres będący wielokrotnością długości słowa maszynowego");
                       is_allocated++;
                       break;
                     }
                 }
                 else if (rand_value < 50)
                 {
                   for (int j = 0; j < 2140; ++j)
                     if (ptr_state[j] == 0)
                     {
                       ptr_state[j] = 1;
                       ptr[j] = heap_calloc_debug(rand() % 1000 + 500, rand() % 4 + 1, __LINE__, __FILE__);  
                       test_error((intptr_t)ptr[j] % sizeof(void *) == 0, "Funkcja heap_calloc_debug() powinien zwrócić adres będący wielokrotnością długości słowa maszynowego");
                       is_allocated++;
                       break;
                     }
                 }
                 else if (rand_value < 60)
                 {
                   for (int j = 0; j < 2140; ++j)
                     if (ptr_state[j] == 0)
                     {
                       ptr_state[j] = 1;
                       ptr[j] = heap_malloc_debug(rand() % 1000 + 500, __LINE__, __FILE__);  
                       test_error((intptr_t)ptr[j] % sizeof(void *) == 0, "Funkcja heap_malloc_debug() powinien zwrócić adres będący wielokrotnością długości słowa maszynowego");
                       is_allocated++;
                       break;
                     }
                 }
                 else if (rand_value < 70)
                 {
                   if (is_allocated)
                   {
                      int to_reallocate = rand() % is_allocated;
                      for (int j = 0; j < 2140; ++j)
                       {
                           if (ptr_state[j] == 1 && !to_reallocate)
                           {
                               if (rand() % 100 < 50)
                                   ptr[j] = heap_realloc_debug(ptr[j], rand() % 100 + 50, __LINE__, __FILE__);
                               else
                                   ptr[j] = heap_realloc_debug(ptr[j], rand() % 1000 + 500, __LINE__, __FILE__);    
                               test_error((intptr_t)ptr[j] % sizeof(void *) == 0, "Funkcja heap_realloc_debug() powinien zwrócić adres będący wielokrotnością długości słowa maszynowego");
                               break;
                           }
                           to_reallocate--;
                       }
                   }
                 }
                 else if (rand_value < 80)
                 {
                   if (is_allocated)
                   {
                     int to_free = rand() % is_allocated;
                      for (int j = 0; j < 2140; ++j)
                       {
                           if (ptr_state[j] == 1 && !to_free)
                           {
                             ptr_state[j] = 0;
                             is_allocated--;
                             heap_realloc_aligned_debug(ptr[j], 0, __LINE__, __FILE__);
                             break;
                           }
                           to_free--;
                       }
                   }
                 }
                 else if (rand_value < 90)
                 {
                   if (is_allocated)
                   {
                     int to_free = rand() % is_allocated;
                      for (int j = 0; j < 2140; ++j)
                       {
                           if (ptr_state[j] == 1 && !to_free)
                           {
                             ptr_state[j] = 0;
                             is_allocated--;
                             heap_realloc_debug(ptr[j], 0, __LINE__, __FILE__);
                             break;
                           }
                           to_free--;
                       }
                   }
                 }
                 else 
                 {
                   if (is_allocated)
                   {
                     int to_free = rand() % is_allocated;
                      for (int j = 0; j < 2140; ++j)
                       {
                           if (ptr_state[j] == 1 && !to_free)
                           {
                             ptr_state[j] = 0;
                             is_allocated--;
                             heap_free(ptr[j]);
                             break;
                           }
                           to_free--;
                       }
                   }
                 }

                 int status = heap_validate();
                 test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);           
               }

               for (int j = 0; j < 2140; ++j)
                 if (ptr_state[j] == 1)
                     heap_realloc_aligned(ptr[j], 0);



            
            return a;
        }

        


//
//  Test 1: Sprawdzanie poprawności działania funkcji heap_malloc - test sprawdza poprawność działania funkcji w przypadku przekazania do niej wartości 0
//
void UTEST1(void)
{
    // informacje o teście
    test_start(1, "Sprawdzanie poprawności działania funkcji heap_malloc - test sprawdza poprawność działania funkcji w przypadku przekazania do niej wartości 0", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);
                
                void *ptr = heap_malloc(0);
                
                test_error(ptr == NULL, "Funkcja heap_malloc() powinna zwrócić wartość NULL");

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
        
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 2: Sprawdzanie poprawności działania funkcji heap_malloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni większej niż przestrzeń obecna na stercie
//
void UTEST2(void)
{
    // informacje o teście
    test_start(2, "Sprawdzanie poprawności działania funkcji heap_malloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni większej niż przestrzeń obecna na stercie", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr = heap_malloc(67108845);

                test_error(ptr == NULL, "Funkcja heap_malloc() powinna zwrócić wartość NULL");

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                        
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 3: Sprawdzanie poprawności działania funkcji heap_malloc - test sprawdza poprawność działania funkcji w przypadku pierwszej próby zaalokowania przestrzeni mniejszej niż przestrzeń obecna na stercie
//
void UTEST3(void)
{
    // informacje o teście
    test_start(3, "Sprawdzanie poprawności działania funkcji heap_malloc - test sprawdza poprawność działania funkcji w przypadku pierwszej próby zaalokowania przestrzeni mniejszej niż przestrzeń obecna na stercie", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    rldebug_heap_set_global_limit(0);
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr = heap_malloc(617);

                test_error(ptr != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr));

                test_error((intptr_t)ptr % sizeof(void *) == 0, "Funkcja heap_malloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                heap_clean();
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 4: Sprawdzanie poprawności działania funkcji heap_malloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni mniejszej niż przestrzeń obecna na stercie, w przypadku przydzielenia wcześniej pamięci użytkownikowi
//
void UTEST4(void)
{
    // informacje o teście
    test_start(4, "Sprawdzanie poprawności działania funkcji heap_malloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni mniejszej niż przestrzeń obecna na stercie, w przypadku przydzielenia wcześniej pamięci użytkownikowi", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    rldebug_heap_set_global_limit(0);
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr1 = heap_malloc(49);
                test_error(ptr1 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr1), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr1));
                test_error((intptr_t)ptr1 % sizeof(void *) == 0, "Funkcja heap_malloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr2 = heap_malloc(173);
                test_error(ptr2 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr2), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr2));
                test_error((intptr_t)ptr2 % sizeof(void *) == 0, "Funkcja heap_malloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                heap_clean();
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 5: Sprawdzanie poprawności działania funkcji heap_malloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni mniejszej niż przestrzeń obecna na stercie (żądana pamięć wymaga zwiększenia limitu pamięci przydzielonego przez system), w przypadku przydzielenia wcześniej pamięci użytkownikowi
//
void UTEST5(void)
{
    // informacje o teście
    test_start(5, "Sprawdzanie poprawności działania funkcji heap_malloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni mniejszej niż przestrzeń obecna na stercie (żądana pamięć wymaga zwiększenia limitu pamięci przydzielonego przez system), w przypadku przydzielenia wcześniej pamięci użytkownikowi", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    rldebug_heap_set_global_limit(0);
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr1 = heap_malloc(30);
                test_error(ptr1 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr1), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr1));
                test_error((intptr_t)ptr1 % sizeof(void *) == 0, "Funkcja heap_malloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr2 = heap_malloc(10471437);
                test_error(ptr2 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr2), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr2));
                test_error((intptr_t)ptr2 % sizeof(void *) == 0, "Funkcja heap_malloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");
                
                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                        
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 6: Sprawdzanie poprawności działania funkcji heap_malloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni większej niż przestrzeń, która może być przydzielona przez system (żądana pamięć wymaga zwiększenia limitu pamięci przydzielonego przez system), w przypadku przydzielenia wcześniej pamięci użytkownikowi
//
void UTEST6(void)
{
    // informacje o teście
    test_start(6, "Sprawdzanie poprawności działania funkcji heap_malloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni większej niż przestrzeń, która może być przydzielona przez system (żądana pamięć wymaga zwiększenia limitu pamięci przydzielonego przez system), w przypadku przydzielenia wcześniej pamięci użytkownikowi", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    rldebug_heap_set_global_limit(0);
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr1 = heap_malloc(82);
                test_error(ptr1 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr1), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr1));
                test_error((intptr_t)ptr1 % sizeof(void *) == 0, "Funkcja heap_malloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");
                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr2 = heap_malloc(67700931);
                test_error(ptr2 == NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                        
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 7: Sprawdzanie poprawności działania funkcji heap_malloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeń mniejszej niż przestrzeń poprzednio zwolniona
//
void UTEST7(void)
{
    // informacje o teście
    test_start(7, "Sprawdzanie poprawności działania funkcji heap_malloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeń mniejszej niż przestrzeń poprzednio zwolniona", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    rldebug_heap_set_global_limit(0);
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr1 = heap_malloc(79);
                void *ptr2 = heap_malloc(103);
                void *ptr3 = heap_malloc(260);

                test_error(ptr1 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(ptr2 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(ptr3 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");

                test_error(pointer_valid == get_pointer_type(ptr1), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr1));
                test_error(pointer_valid == get_pointer_type(ptr2), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr2));
                test_error(pointer_valid == get_pointer_type(ptr3), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr3));

                test_error((intptr_t)ptr1 % sizeof(void *) == 0, "Funkcja heap_malloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");
                test_error((intptr_t)ptr2 % sizeof(void *) == 0, "Funkcja heap_malloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");
                test_error((intptr_t)ptr3 % sizeof(void *) == 0, "Funkcja heap_malloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                heap_free(ptr2);

                test_error(pointer_unallocated == get_pointer_type(ptr2), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_unallocated, a zwróciła %d", get_pointer_type(ptr2));

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr4 = heap_malloc(68);
                test_error(ptr2 == ptr4, "Funkcja heap_malloc() powinna zwrócić adres równy adresowi ptr2 (jest możliwość wykorzystania wolnego bloku pamięci do zaalokowania), a zwróciła różny");

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 8: Sprawdzanie poprawności działania funkcji heap_malloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni równej przestrzeni poprzednio zwolnionej
//
void UTEST8(void)
{
    // informacje o teście
    test_start(8, "Sprawdzanie poprawności działania funkcji heap_malloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni równej przestrzeni poprzednio zwolnionej", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    rldebug_heap_set_global_limit(0);
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr1 = heap_malloc(60);
                void *ptr2 = heap_malloc(117);
                void *ptr3 = heap_malloc(296);

                test_error(ptr1 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(ptr2 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(ptr3 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");

                test_error(pointer_valid == get_pointer_type(ptr1), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr1));
                test_error(pointer_valid == get_pointer_type(ptr2), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr2));
                test_error(pointer_valid == get_pointer_type(ptr3), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr3));

                test_error((intptr_t)ptr1 % sizeof(void *) == 0, "Funkcja heap_malloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");
                test_error((intptr_t)ptr2 % sizeof(void *) == 0, "Funkcja heap_malloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");
                test_error((intptr_t)ptr3 % sizeof(void *) == 0, "Funkcja heap_malloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                heap_free(ptr2);

                test_error(pointer_unallocated == get_pointer_type(ptr2), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_unallocated, a zwróciła %d", get_pointer_type(ptr2));

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr4 = heap_malloc(117);
                test_error(ptr2 == ptr4, "Funkcja heap_malloc() powinna zwrócić adres równy adresowi ptr2 (jest możliwość wykorzystania wolnego bloku pamięci do zaalokowania), a zwróciła różny");

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 9: Sprawdzanie poprawności działania funkcji heap_malloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni mniejszej niż przestrzeń poprzednio zwolniona
//
void UTEST9(void)
{
    // informacje o teście
    test_start(9, "Sprawdzanie poprawności działania funkcji heap_malloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni mniejszej niż przestrzeń poprzednio zwolniona", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    rldebug_heap_set_global_limit(0);
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                size_t mem_sizes[] = {209, 535, 173, 639, 944, 484, 696, 962, 501, 228, 976, 162, 238, 255, 169, 844, 527, 827, 456, 289, 639};
                void *ptrs[21];

                for (int i = 0; i < 21; ++i)
                {
                    ptrs[i] = heap_malloc(mem_sizes[i]);
                    test_error(ptrs[i] != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                    test_error(pointer_valid == get_pointer_type(ptrs[i]), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptrs[i]));
                    test_error((intptr_t)ptrs[i] % sizeof(void *) == 0, "Funkcja heap_malloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");
        
                    status = heap_validate();
                    test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                }           

                heap_free(ptrs[13]);

                test_error(pointer_unallocated == get_pointer_type(ptrs[13]), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_unallocated, a zwróciła %d", get_pointer_type(ptrs[13]));

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr = heap_malloc(249);
                test_error(ptrs[13] == ptr, "Funkcja heap_malloc() powinna zwrócić adres równy adresowi ptrs[13] (jest możliwość wykorzystania wolnego bloku pamięci do zaalokowania), a zwróciła różny");

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 10: Sprawdzanie poprawności działania funkcji heap_malloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni równej przestrzeń poprzednio zwolnionej
//
void UTEST10(void)
{
    // informacje o teście
    test_start(10, "Sprawdzanie poprawności działania funkcji heap_malloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni równej przestrzeń poprzednio zwolnionej", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    rldebug_heap_set_global_limit(0);
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                size_t mem_sizes[] = {108, 910, 292, 813, 443, 225, 823, 612, 177, 779, 283, 685, 433, 831, 203, 207, 458, 213, 339, 609, 781, 265, 421, 388, 631, 217, 785, 404, 445, 290, 120, 956, 293, 107, 415, 744, 216, 356, 730, 596};
                void *ptrs[40];

                for (int i = 0; i < 40; ++i)
                {
                    ptrs[i] = heap_malloc(mem_sizes[i]);
                    test_error(ptrs[i] != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                    test_error(pointer_valid == get_pointer_type(ptrs[i]), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptrs[i]));
                    test_error((intptr_t)ptrs[i] % sizeof(void *) == 0, "Funkcja heap_malloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");

                    status = heap_validate();
                    test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                }           

                heap_free(ptrs[25]);

                test_error(pointer_unallocated == get_pointer_type(ptrs[25]), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_unallocated, a zwróciła %d", get_pointer_type(ptrs[25]));

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr = heap_malloc(217);
                test_error(ptrs[25] == ptr, "Funkcja heap_malloc() powinna zwrócić adres równy adresowi ptrs[25] (jest możliwość wykorzystania wolnego bloku pamięci do zaalokowania), a zwróciła różny");

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 11: Sprawdzanie poprawności działania funkcji heap_malloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni większej niż przestrzeń poprzednio zwolnionej
//
void UTEST11(void)
{
    // informacje o teście
    test_start(11, "Sprawdzanie poprawności działania funkcji heap_malloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni większej niż przestrzeń poprzednio zwolnionej", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    rldebug_heap_set_global_limit(0);
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr1 = heap_malloc(69);
                void *ptr2 = heap_malloc(174);
                void *ptr3 = heap_malloc(247);

                test_error(ptr1 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(ptr2 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(ptr3 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");

                test_error(pointer_valid == get_pointer_type(ptr1), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr1));
                test_error(pointer_valid == get_pointer_type(ptr2), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr2));
                test_error(pointer_valid == get_pointer_type(ptr3), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr3));

                test_error((intptr_t)ptr1 % sizeof(void *) == 0, "Funkcja heap_malloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");
                test_error((intptr_t)ptr2 % sizeof(void *) == 0, "Funkcja heap_malloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");
                test_error((intptr_t)ptr3 % sizeof(void *) == 0, "Funkcja heap_malloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                heap_free(ptr2);

                test_error(pointer_unallocated == get_pointer_type(ptr2), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_unallocated, a zwróciła %d", get_pointer_type(ptr2));

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr4 = heap_malloc(182);
                
                test_error(ptr2 != ptr4, "Funkcja heap_malloc() powinna zwrócić adres równy adresowi ptr2 (jest możliwość wykorzystania wolnego bloku pamięci do zaalokowania), a zwróciła różny");
                test_error(ptr4 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr4), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr4));
                test_error((intptr_t)ptr4 % sizeof(void *) == 0, "Funkcja heap_malloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");


                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                        
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 12: Sprawdzanie poprawności działania funkcji heap_malloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni większej niż przestrzeń poprzednio zwolnionej
//
void UTEST12(void)
{
    // informacje o teście
    test_start(12, "Sprawdzanie poprawności działania funkcji heap_malloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni większej niż przestrzeń poprzednio zwolnionej", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    rldebug_heap_set_global_limit(0);
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr1 = heap_malloc(64);
                void *ptr2 = heap_malloc(6);
                void *ptr3 = heap_malloc(265);

                test_error(ptr1 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(ptr2 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(ptr3 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");

                test_error(pointer_valid == get_pointer_type(ptr1), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr1));
                test_error(pointer_valid == get_pointer_type(ptr2), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr2));
                test_error(pointer_valid == get_pointer_type(ptr3), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr3));

                test_error((intptr_t)ptr1 % sizeof(void *) == 0, "Funkcja heap_malloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");
                test_error((intptr_t)ptr2 % sizeof(void *) == 0, "Funkcja heap_malloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");
                test_error((intptr_t)ptr3 % sizeof(void *) == 0, "Funkcja heap_malloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                heap_free(ptr2);

                test_error(pointer_unallocated == get_pointer_type(ptr2), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_unallocated, a zwróciła %d", get_pointer_type(ptr2));

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr4 = heap_malloc(16);

                test_error(ptr2 != ptr4, "Funkcja heap_malloc() powinna zwrócić adres równy adresowi ptr2 (jest możliwość wykorzystania wolnego bloku pamięci do zaalokowania), a zwróciła różny");
                test_error(ptr4 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr4), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr4));
                test_error((intptr_t)ptr4 % sizeof(void *) == 0, "Funkcja heap_malloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                
        
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 13: Sprawdzanie poprawności działania funkcji heap_malloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni większej niż przestrzeń poprzednio zwolnionej
//
void UTEST13(void)
{
    // informacje o teście
    test_start(13, "Sprawdzanie poprawności działania funkcji heap_malloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni większej niż przestrzeń poprzednio zwolnionej", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    rldebug_heap_set_global_limit(0);
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr1 = heap_malloc(18);
                void *ptr2 = heap_malloc(1153);
                void *ptr3 = heap_malloc(244);

                test_error(ptr1 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(ptr2 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(ptr3 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");

                test_error(pointer_valid == get_pointer_type(ptr1), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr1));
                test_error(pointer_valid == get_pointer_type(ptr2), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr2));
                test_error(pointer_valid == get_pointer_type(ptr3), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr3));

                test_error((intptr_t)ptr1 % sizeof(void *) == 0, "Funkcja heap_malloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");
                test_error((intptr_t)ptr2 % sizeof(void *) == 0, "Funkcja heap_malloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");
                test_error((intptr_t)ptr3 % sizeof(void *) == 0, "Funkcja heap_malloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                heap_free(ptr2);

                test_error(pointer_unallocated == get_pointer_type(ptr2), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_unallocated, a zwróciła %d", get_pointer_type(ptr2));

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr4 = heap_malloc(108);

                test_error(ptr2 == ptr4, "Funkcja heap_malloc() powinna zwrócić adres równy adresowi ptr2 (jest możliwość wykorzystania wolnego bloku pamięci do zaalokowania), a zwróciła różny");

                void *ptr5 = heap_malloc(181);
                test_error(ptr2 != ptr5, "Funkcja heap_malloc() powinna zwrócić adres równy adresowi ptr2 (jest możliwość wykorzystania wolnego bloku pamięci do zaalokowania), a zwróciła różny");
                test_error(pointer_valid == get_pointer_type(ptr5), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr5));
                test_error((intptr_t)ptr5 % sizeof(void *) == 0, "Funkcja heap_malloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");
                test_error((intptr_t)ptr5 > (intptr_t)ptr4 && (intptr_t)ptr5 < (intptr_t)ptr3, "Funkcja heap_malloc() powinna zwrócić adres znajdujący się pomiędzy blokami wcześniej przydzielonymi");

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);

        
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 14: Sprawdzanie poprawności działania funkcji heap_calloc - test sprawdza poprawność działania funkcji w przypadku przekazania do niej wartości 0
//
void UTEST14(void)
{
    // informacje o teście
    test_start(14, "Sprawdzanie poprawności działania funkcji heap_calloc - test sprawdza poprawność działania funkcji w przypadku przekazania do niej wartości 0", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr = heap_calloc(0, 0);

                test_error(ptr == NULL, "Funkcja heap_calloc() powinna zwrócić wartość NULL");

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                
                
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 15: Sprawdzanie poprawności działania funkcji heap_calloc - test sprawdza poprawność działania funkcji w przypadku przekazania do niej wartości 0
//
void UTEST15(void)
{
    // informacje o teście
    test_start(15, "Sprawdzanie poprawności działania funkcji heap_calloc - test sprawdza poprawność działania funkcji w przypadku przekazania do niej wartości 0", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr = heap_calloc(20, 0);

                test_error(ptr == NULL, "Funkcja heap_calloc() powinna zwrócić wartość NULL");

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                
        
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 16: Sprawdzanie poprawności działania funkcji heap_calloc - test sprawdza poprawność działania funkcji w przypadku przekazania do niej wartości 0
//
void UTEST16(void)
{
    // informacje o teście
    test_start(16, "Sprawdzanie poprawności działania funkcji heap_calloc - test sprawdza poprawność działania funkcji w przypadku przekazania do niej wartości 0", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr = heap_calloc(0, 1);

                test_error(ptr == NULL, "Funkcja heap_calloc() powinna zwrócić wartość NULL");

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 17: Sprawdzanie poprawności działania funkcji heap_calloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni większej niż przestrzeń obecna na stercie
//
void UTEST17(void)
{
    // informacje o teście
    test_start(17, "Sprawdzanie poprawności działania funkcji heap_calloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni większej niż przestrzeń obecna na stercie", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr = heap_calloc(67108835, 1);

                test_error(ptr == NULL, "Funkcja heap_calloc() powinna zwrócić wartość NULL");

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 18: Sprawdzanie poprawności działania funkcji heap_calloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni większej niż przestrzeń obecna na stercie
//
void UTEST18(void)
{
    // informacje o teście
    test_start(18, "Sprawdzanie poprawności działania funkcji heap_calloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni większej niż przestrzeń obecna na stercie", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr = heap_calloc(16777209, 4);

                test_error(ptr == NULL, "Funkcja heap_calloc() powinna zwrócić wartość NULL");

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 19: Sprawdzanie poprawności działania funkcji heap_calloc - test sprawdza poprawność działania funkcji w przypadku pierwszej próby zaalokowania przestrzeni mniejszej niż przestrzeń obecna na stercie
//
void UTEST19(void)
{
    // informacje o teście
    test_start(19, "Sprawdzanie poprawności działania funkcji heap_calloc - test sprawdza poprawność działania funkcji w przypadku pierwszej próby zaalokowania przestrzeni mniejszej niż przestrzeń obecna na stercie", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    rldebug_heap_set_global_limit(0);
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                char *ptr = heap_calloc(599, 1);

                test_error(ptr != NULL, "Funkcja heap_calloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr), "Funkcja heap_calloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr));
                test_error((intptr_t)ptr % sizeof(void *) == 0, "Funkcja heap_malloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");

                for (int i = 0; i < 599; ++i)
                    test_error(ptr[i] == 0, "Funkcja heap_calloc() powinna zwrócić wskaźnik na obszar pamięci wypełniony wartościami 0");


                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 20: Sprawdzanie poprawności działania funkcji heap_calloc - test sprawdza poprawność działania funkcji w przypadku pierwszej próby zaalokowania przestrzeni mniejszej niż przestrzeń obecna na stercie
//
void UTEST20(void)
{
    // informacje o teście
    test_start(20, "Sprawdzanie poprawności działania funkcji heap_calloc - test sprawdza poprawność działania funkcji w przypadku pierwszej próby zaalokowania przestrzeni mniejszej niż przestrzeń obecna na stercie", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    rldebug_heap_set_global_limit(0);
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                char *ptr = heap_calloc(146, 4);

                test_error(ptr != NULL, "Funkcja heap_calloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr), "Funkcja heap_calloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr));
                test_error((intptr_t)ptr % sizeof(void *) == 0, "Funkcja heap_malloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");

                for (int i = 0; i < 584; ++i)
                    test_error(ptr[i] == 0, "Funkcja heap_calloc() powinna zwrócić wskaźnik na obszar pamięci wypełniony wartościami 0");


                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 21: Sprawdzanie poprawności działania funkcji heap_calloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni mniejszej niż przestrzeń obecna na stercie, w przypadku przydzielenia wcześniej pamięci użytkownikowi
//
void UTEST21(void)
{
    // informacje o teście
    test_start(21, "Sprawdzanie poprawności działania funkcji heap_calloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni mniejszej niż przestrzeń obecna na stercie, w przypadku przydzielenia wcześniej pamięci użytkownikowi", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    rldebug_heap_set_global_limit(0);
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                char *ptr1 = heap_calloc(65, 92);
                test_error(ptr1 != NULL, "Funkcja heap_calloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr1), "Funkcja heap_calloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr1));
                test_error((intptr_t)ptr1 % sizeof(void *) == 0, "Funkcja heap_malloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                for (int i = 0; i < 5980; ++i)
                    test_error(ptr1[i] == 0, "Funkcja heap_calloc() powinna zwrócić wskaźnik na obszar pamięci wypełniony wartościami 0");

                char *ptr2 = heap_calloc(127, 92);
                test_error(ptr2 != NULL, "Funkcja heap_calloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr2), "Funkcja heap_calloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr2));
                test_error((intptr_t)ptr2 % sizeof(void *) == 0, "Funkcja heap_malloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                for (int i = 0; i < 11684; ++i)
                    test_error(ptr2[i] == 0, "Funkcja heap_calloc() powinna zwrócić wskaźnik na obszar pamięci wypełniony wartościami 0");

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 22: Sprawdzanie poprawności działania funkcji heap_calloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni mniejszej niż przestrzeń obecna na stercie (żądana pamięć wymaga zwiększenia limitu pamięci przydzielonego przez system), w przypadku przydzielenia wcześniej pamięci użytkownikowi
//
void UTEST22(void)
{
    // informacje o teście
    test_start(22, "Sprawdzanie poprawności działania funkcji heap_calloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni mniejszej niż przestrzeń obecna na stercie (żądana pamięć wymaga zwiększenia limitu pamięci przydzielonego przez system), w przypadku przydzielenia wcześniej pamięci użytkownikowi", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    rldebug_heap_set_global_limit(0);
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                char *ptr1 = heap_calloc(50, 1);
                test_error(ptr1 != NULL, "Funkcja heap_calloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr1), "Funkcja heap_calloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr1));
                test_error((intptr_t)ptr1 % sizeof(void *) == 0, "Funkcja heap_malloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");
                
                
                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                for (int i = 0; i < 50; ++i)
                    test_error(ptr1[i] == 0, "Funkcja heap_calloc() powinna zwrócić wskaźnik na obszar pamięci wypełniony wartościami 0");

                char *ptr2 = heap_calloc(11345858, 1);
                test_error(ptr2 != NULL, "Funkcja heap_calloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr2), "Funkcja heap_calloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr2));
                test_error((intptr_t)ptr2 % sizeof(void *) == 0, "Funkcja heap_malloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                for (int i = 0; i < 11345858; ++i)
                    test_error(ptr2[i] == 0, "Funkcja heap_calloc() powinna zwrócić wskaźnik na obszar pamięci wypełniony wartościami 0");

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 23: Sprawdzanie poprawności działania funkcji heap_calloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni większej niż przestrzeń, która może być przydzielona przez system (żądana pamięć wymaga zwiększenia limitu pamięci przydzielonego przez system), w przypadku przydzielenia wcześniej pamięci użytkownikowi
//
void UTEST23(void)
{
    // informacje o teście
    test_start(23, "Sprawdzanie poprawności działania funkcji heap_calloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni większej niż przestrzeń, która może być przydzielona przez system (żądana pamięć wymaga zwiększenia limitu pamięci przydzielonego przez system), w przypadku przydzielenia wcześniej pamięci użytkownikowi", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    rldebug_heap_set_global_limit(0);
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                char *ptr1 = heap_calloc(54, 3);
                test_error(ptr1 != NULL, "Funkcja heap_calloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr1), "Funkcja heap_calloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr1));
                test_error((intptr_t)ptr1 % sizeof(void *) == 0, "Funkcja heap_malloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                for (int i = 0; i < 162; ++i)
                    test_error(ptr1[i] == 0, "Funkcja heap_calloc() powinna zwrócić wskaźnik na obszar pamięci wypełniony wartościami 0");

                char *ptr2 = heap_calloc(67142798, 3);
                test_error(ptr2 == NULL, "Funkcja heap_calloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 24: Sprawdzanie poprawności działania funkcji heap_calloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeń mniejszej niż przestrzeń poprzednio zwolniona
//
void UTEST24(void)
{
    // informacje o teście
    test_start(24, "Sprawdzanie poprawności działania funkcji heap_calloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeń mniejszej niż przestrzeń poprzednio zwolniona", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    rldebug_heap_set_global_limit(0);
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                char *ptr1 = heap_calloc(63, 1);
                char *ptr2 = heap_calloc(144, 1);
                char *ptr3 = heap_calloc(233, 1);

                test_error(ptr1 != NULL, "Funkcja heap_calloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(ptr2 != NULL, "Funkcja heap_calloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(ptr3 != NULL, "Funkcja heap_calloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");

                test_error(pointer_valid == get_pointer_type(ptr1), "Funkcja heap_calloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr1));
                test_error(pointer_valid == get_pointer_type(ptr2), "Funkcja heap_calloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr2));
                test_error(pointer_valid == get_pointer_type(ptr3), "Funkcja heap_calloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr3));

                test_error((intptr_t)ptr1 % sizeof(void *) == 0, "Funkcja heap_malloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");
                test_error((intptr_t)ptr2 % sizeof(void *) == 0, "Funkcja heap_malloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");
                test_error((intptr_t)ptr3 % sizeof(void *) == 0, "Funkcja heap_malloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");

                for (int i = 0; i < 63; ++i)
                    test_error(ptr1[i] == 0, "Funkcja heap_calloc() powinna zwrócić wskaźnik na obszar pamięci wypełniony wartościami 0");

                for (int i = 0; i < 144; ++i)
                    test_error(ptr2[i] == 0, "Funkcja heap_calloc() powinna zwrócić wskaźnik na obszar pamięci wypełniony wartościami 0");
     
                for (int i = 0; i < 233; ++i)
                    test_error(ptr3[i] == 0, "Funkcja heap_calloc() powinna zwrócić wskaźnik na obszar pamięci wypełniony wartościami 0");

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                heap_free(ptr2);

                test_error(pointer_unallocated == get_pointer_type(ptr2), "Funkcja heap_calloc() powinna zwrócić wskaźnik typu pointer_unallocated, a zwróciła %d", get_pointer_type(ptr2));

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                char *ptr4 = heap_calloc(130, 1);
                test_error(ptr2 == ptr4, "Funkcja heap_calloc() powinna zwrócić adres równy adresowi ptr2 (jest możliwość wykorzystania wolnego bloku pamięci do zaalokowania), a zwróciła różny");

                for (int i = 0; i < 130; ++i)
                    test_error(ptr4[i] == 0, "Funkcja heap_calloc() powinna zwrócić wskaźnik na obszar pamięci wypełniony wartościami 0");

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 25: Sprawdzanie poprawności działania funkcji heap_calloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni równej przestrzeni poprzednio zwolnionej
//
void UTEST25(void)
{
    // informacje o teście
    test_start(25, "Sprawdzanie poprawności działania funkcji heap_calloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni równej przestrzeni poprzednio zwolnionej", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    rldebug_heap_set_global_limit(0);
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                char *ptr1 = heap_calloc(22, 4);
                char *ptr2 = heap_calloc(142, 4);
                char *ptr3 = heap_calloc(244, 4);

                test_error(ptr1 != NULL, "Funkcja heap_calloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(ptr2 != NULL, "Funkcja heap_calloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(ptr3 != NULL, "Funkcja heap_calloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");

                test_error(pointer_valid == get_pointer_type(ptr1), "Funkcja heap_calloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr1));
                test_error(pointer_valid == get_pointer_type(ptr2), "Funkcja heap_calloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr2));
                test_error(pointer_valid == get_pointer_type(ptr3), "Funkcja heap_calloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr3));

                test_error((intptr_t)ptr1 % sizeof(void *) == 0, "Funkcja heap_malloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");
                test_error((intptr_t)ptr2 % sizeof(void *) == 0, "Funkcja heap_malloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");
                test_error((intptr_t)ptr3 % sizeof(void *) == 0, "Funkcja heap_malloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");


                for (int i = 0; i < 88; ++i)
                    test_error(ptr1[i] == 0, "Funkcja heap_calloc() powinna zwrócić wskaźnik na obszar pamięci wypełniony wartościami 0");

                for (int i = 0; i < 568; ++i)
                    test_error(ptr2[i] == 0, "Funkcja heap_calloc() powinna zwrócić wskaźnik na obszar pamięci wypełniony wartościami 0");
     
                for (int i = 0; i < 976; ++i)
                    test_error(ptr3[i] == 0, "Funkcja heap_calloc() powinna zwrócić wskaźnik na obszar pamięci wypełniony wartościami 0");

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                heap_free(ptr2);

                test_error(pointer_unallocated == get_pointer_type(ptr2), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_unallocated, a zwróciła %d", get_pointer_type(ptr2));

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                char *ptr4 = heap_calloc(142, 4);
                test_error(ptr2 == ptr4, "Funkcja heap_calloc() powinna zwrócić adres równy adresowi ptr2 (jest możliwość wykorzystania wolnego bloku pamięci do zaalokowania), a zwróciła różny");

                for (int i = 0; i < 568; ++i)
                    test_error(ptr4[i] == 0, "Funkcja heap_calloc() powinna zwrócić wskaźnik na obszar pamięci wypełniony wartościami 0");

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 26: Sprawdzanie poprawności działania funkcji heap_calloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni mniejszej niż przestrzeń poprzednio zwolniona
//
void UTEST26(void)
{
    // informacje o teście
    test_start(26, "Sprawdzanie poprawności działania funkcji heap_calloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni mniejszej niż przestrzeń poprzednio zwolniona", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    rldebug_heap_set_global_limit(0);
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                size_t mem_sizes[] = {343, 219, 578, 667, 857, 614, 852, 699, 567, 602, 104, 287, 457, 360, 656, 302, 448, 588, 448, 730, 337, 382, 443, 693, 606, 544, 511, 291};
                size_t mem_units[] = {3, 4, 4, 3, 5, 3, 4, 2, 1, 1, 1, 5, 4, 1, 3, 5, 1, 5, 3, 3, 5, 4, 2, 4, 1, 5, 2, 2};
                char *ptrs[28];

                for (int i = 0; i < 28; ++i)
                {
                    ptrs[i] = heap_calloc(mem_sizes[i], mem_units[i]);
                    test_error(ptrs[i] != NULL, "Funkcja heap_calloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                    test_error(pointer_valid == get_pointer_type(ptrs[i]), "Funkcja heap_calloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptrs[i]));

                    test_error((intptr_t)ptrs[i] % sizeof(void *) == 0, "Funkcja heap_malloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");

                    for (size_t j = 0; j < mem_sizes[i] * mem_units[i]; ++j)
                        test_error(ptrs[i][j] == 0, "Funkcja heap_calloc() powinna zwrócić wskaźnik na obszar pamięci wypełniony wartościami 0");

                    status = heap_validate();
                    test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                }

                heap_free(ptrs[16]);

                test_error(pointer_unallocated == get_pointer_type(ptrs[16]), "Funkcja heap_calloc() powinna zwrócić wskaźnik typu pointer_unallocated, a zwróciła %d", get_pointer_type(ptrs[16]));

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr = heap_calloc(434, 1);
                test_error(ptrs[16] == ptr, "Funkcja heap_calloc() powinna zwrócić adres równy adresowi ptrs[16] (jest możliwość wykorzystania wolnego bloku pamięci do zaalokowania), a zwróciła różny");

                for (size_t j = 0; j < 434 * 1; ++j)
                    test_error(ptrs[16][j] == 0, "Funkcja heap_calloc() powinna zwrócić wskaźnik na obszar pamięci wypełniony wartościami 0");


                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
             
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 27: Sprawdzanie poprawności działania funkcji heap_calloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni równej przestrzeni poprzednio zwolnionej
//
void UTEST27(void)
{
    // informacje o teście
    test_start(27, "Sprawdzanie poprawności działania funkcji heap_calloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni równej przestrzeni poprzednio zwolnionej", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    rldebug_heap_set_global_limit(0);
    
    //
    // -----------
    //
    
                 int status = heap_setup();
                 test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                 size_t mem_sizes[] = {330, 620, 414, 742, 607, 246, 654, 586, 202, 282, 281, 377, 825, 108, 564, 750, 763, 582, 805, 113, 319, 905, 393, 299, 502, 405, 929, 473};
                 size_t mem_units[] = {1, 5, 2, 1, 5, 4, 4, 4, 3, 5, 1, 1, 5, 4, 3, 4, 4, 1, 5, 3, 4, 5, 4, 1, 5, 5, 1, 1};
                 char *ptrs[28];

                 for (int i = 0; i < 28; ++i)
                 {
                     ptrs[i] = heap_calloc(mem_sizes[i], mem_units[i]);
                     test_error(ptrs[i] != NULL, "Funkcja heap_calloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                     test_error(pointer_valid == get_pointer_type(ptrs[i]), "Funkcja heap_calloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptrs[i]));
                     test_error((intptr_t)ptrs[i] % sizeof(void *) == 0, "Funkcja heap_malloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");

                     for (size_t j = 0; j < mem_sizes[i] * mem_units[i]; ++j)
                         test_error(ptrs[i][j] == 0, "Funkcja heap_calloc() powinna zwrócić wskaźnik na obszar pamięci wypełniony wartościami 0");

                     status = heap_validate();
                     test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                 }

                 heap_free(ptrs[10]);

                 test_error(pointer_unallocated == get_pointer_type(ptrs[10]), "Funkcja heap_calloc() powinna zwrócić wskaźnik typu pointer_unallocated, a zwróciła %d", get_pointer_type(ptrs[10]));

                 status = heap_validate();
                 test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                 void *ptr = heap_calloc(281, 1);
                 test_error(ptrs[10] == ptr, "Funkcja heap_calloc() powinna zwrócić adres równy adresowi ptrs[10] (jest możliwość wykorzystania wolnego bloku pamięci do zaalokowania), a zwróciła różny");

                 for (size_t j = 0; j < 281 * 1; ++j)
                     test_error(ptrs[10][j] == 0, "Funkcja heap_calloc() powinna zwrócić wskaźnik na obszar pamięci wypełniony wartościami 0");


                 status = heap_validate();
                 test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                
             
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 28: Sprawdzanie poprawności działania funkcji heap_calloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni większej niż przestrzeń poprzednio zwolnionej
//
void UTEST28(void)
{
    // informacje o teście
    test_start(28, "Sprawdzanie poprawności działania funkcji heap_calloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni większej niż przestrzeń poprzednio zwolnionej", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    rldebug_heap_set_global_limit(0);
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                char *ptr1 = heap_calloc(75, 1);
                char *ptr2 = heap_calloc(170, 1);
                char *ptr3 = heap_calloc(200, 1);

                test_error(ptr1 != NULL, "Funkcja heap_calloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(ptr2 != NULL, "Funkcja heap_calloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(ptr3 != NULL, "Funkcja heap_calloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");

                test_error(pointer_valid == get_pointer_type(ptr1), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr1));
                test_error(pointer_valid == get_pointer_type(ptr2), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr2));
                test_error(pointer_valid == get_pointer_type(ptr3), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr3));

                test_error((intptr_t)ptr1 % sizeof(void *) == 0, "Funkcja heap_malloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");
                test_error((intptr_t)ptr2 % sizeof(void *) == 0, "Funkcja heap_malloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");
                test_error((intptr_t)ptr3 % sizeof(void *) == 0, "Funkcja heap_malloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");

                for (int i = 0; i < 75; ++i)
                    test_error(ptr1[i] == 0, "Funkcja heap_calloc() powinna zwrócić wskaźnik na obszar pamięci wypełniony wartościami 0");

                for (int i = 0; i < 170; ++i)
                    test_error(ptr2[i] == 0, "Funkcja heap_calloc() powinna zwrócić wskaźnik na obszar pamięci wypełniony wartościami 0");
     
                for (int i = 0; i < 200; ++i)
                    test_error(ptr3[i] == 0, "Funkcja heap_calloc() powinna zwrócić wskaźnik na obszar pamięci wypełniony wartościami 0");

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                heap_free(ptr2);

                test_error(pointer_unallocated == get_pointer_type(ptr2), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_unallocated, a zwróciła %d", get_pointer_type(ptr2));

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                char *ptr4 = heap_calloc(178, 1);

                test_error(ptr2 != ptr4, "Funkcja heap_calloc() powinna zwrócić adres równy adresowi ptr2 (jest możliwość wykorzystania wolnego bloku pamięci do zaalokowania), a zwróciła różny");
                test_error(ptr4 != NULL, "Funkcja heap_calloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr4), "Funkcja heap_calloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr4));
                test_error((intptr_t)ptr4 % sizeof(void *) == 0, "Funkcja heap_malloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");

                for (int i = 0; i < 178; ++i)
                    test_error(ptr4[i] == 0, "Funkcja heap_calloc() powinna zwrócić wskaźnik na obszar pamięci wypełniony wartościami 0");

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                
        
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 29: Sprawdzanie poprawności działania funkcji heap_calloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni większej niż przestrzeń poprzednio zwolnionej
//
void UTEST29(void)
{
    // informacje o teście
    test_start(29, "Sprawdzanie poprawności działania funkcji heap_calloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni większej niż przestrzeń poprzednio zwolnionej", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    rldebug_heap_set_global_limit(0);
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                char *ptr1 = heap_calloc(24, 1);
                char *ptr2 = heap_calloc(1912, 1);
                char *ptr3 = heap_calloc(290, 1);

                test_error(ptr1 != NULL, "Funkcja heap_calloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(ptr2 != NULL, "Funkcja heap_calloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(ptr3 != NULL, "Funkcja heap_calloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");

                test_error(pointer_valid == get_pointer_type(ptr1), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr1));
                test_error(pointer_valid == get_pointer_type(ptr2), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr2));
                test_error(pointer_valid == get_pointer_type(ptr3), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr3));

                test_error((intptr_t)ptr1 % sizeof(void *) == 0, "Funkcja heap_malloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");
                test_error((intptr_t)ptr2 % sizeof(void *) == 0, "Funkcja heap_malloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");
                test_error((intptr_t)ptr3 % sizeof(void *) == 0, "Funkcja heap_malloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");

                for (int i = 0; i < 24; ++i)
                    test_error(ptr1[i] == 0, "Funkcja heap_calloc() powinna zwrócić wskaźnik na obszar pamięci wypełniony wartościami 0");

                for (int i = 0; i < 1912; ++i)
                    test_error(ptr2[i] == 0, "Funkcja heap_calloc() powinna zwrócić wskaźnik na obszar pamięci wypełniony wartościami 0");

                for (int i = 0; i < 290; ++i)
                    test_error(ptr3[i] == 0, "Funkcja heap_calloc() powinna zwrócić wskaźnik na obszar pamięci wypełniony wartościami 0");

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                heap_free(ptr2);

                test_error(pointer_unallocated == get_pointer_type(ptr2), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_unallocated, a zwróciła %d", get_pointer_type(ptr2));

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                char *ptr4 = heap_calloc(143, 1);

                test_error(ptr2 == ptr4, "Funkcja heap_calloc() powinna zwrócić adres równy adresowi ptr2 (jest możliwość wykorzystania wolnego bloku pamięci do zaalokowania), a zwróciła różny");

                char *ptr5 = heap_calloc(143, 1);

                test_error(ptr5 != NULL, "Funkcja heap_calloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr5), "Funkcja heap_calloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr5));
                test_error((intptr_t)ptr5 % sizeof(void *) == 0, "Funkcja heap_malloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");
                test_error((intptr_t)ptr5 > (intptr_t)ptr4 && (intptr_t)ptr5 < (intptr_t)ptr3, "Funkcja heap_malloc() powinna zwrócić adres znajdujący się pomiędzy blokami wcześniej przydzielonymi");

                for (int i = 0; i < 143; ++i)
                    test_error(ptr4[i] == 0, "Funkcja heap_calloc() powinna zwrócić wskaźnik na obszar pamięci wypełniony wartościami 0");

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);

        
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 30: Sprawdzanie poprawności działania funkcji heap_get_largest_used_block_size
//
void UTEST30(void)
{
    // informacje o teście
    test_start(30, "Sprawdzanie poprawności działania funkcji heap_get_largest_used_block_size", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    rldebug_heap_set_global_limit(0);
    
    //
    // -----------
    //
    

                size_t block_size = heap_get_largest_used_block_size();
                test_error(block_size == 0, "Funkcja heap_get_largest_used_block_size() powinna zwrócić wartość 0, a zwróciła na %lu", block_size);

                heap_clean();
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 31: Sprawdzanie poprawności działania funkcji heap_get_largest_used_block_size
//
void UTEST31(void)
{
    // informacje o teście
    test_start(31, "Sprawdzanie poprawności działania funkcji heap_get_largest_used_block_size", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    rldebug_heap_set_global_limit(0);
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                size_t block_size = heap_get_largest_used_block_size();
                test_error(block_size == 0, "Funkcja heap_get_largest_used_block_size() powinna zwrócić wartość 0, a zwróciła na %lu", block_size);

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                
        
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 32: Sprawdzanie poprawności działania funkcji heap_get_largest_used_block_size
//
void UTEST32(void)
{
    // informacje o teście
    test_start(32, "Sprawdzanie poprawności działania funkcji heap_get_largest_used_block_size", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    rldebug_heap_set_global_limit(0);
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                size_t mem_sizes[] = {942, 440, 747, 286, 896, 782, 753, 128, 546, 616, 967, 519, 177, 376, 672, 533, 821, 577, 429, 895, 465, 802, 740, 342, 784, 685, 866, 262, 406, 661, 943, 797, 488, 521, 998};
                void *ptrs[35];

                for (int i = 0; i < 35; ++i)
                {
                    ptrs[i] = heap_malloc(mem_sizes[i]);
                    test_error(pointer_valid == get_pointer_type(ptrs[i]), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptrs[i]));
                    test_error((intptr_t)ptrs[i] % sizeof(void *) == 0, "Funkcja heap_malloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");

                    status = heap_validate();
                    test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                }           


                size_t block_size = heap_get_largest_used_block_size();
                test_error(block_size == 998, "Funkcja heap_get_largest_used_block_size() powinna zwrócić wartość 998, a zwróciła na %lu", block_size);

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                        
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 33: Sprawdzanie poprawności działania funkcji heap_get_largest_used_block_size
//
void UTEST33(void)
{
    // informacje o teście
    test_start(33, "Sprawdzanie poprawności działania funkcji heap_get_largest_used_block_size", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    rldebug_heap_set_global_limit(0);
    
    //
    // -----------
    //
    
                 int status = heap_setup();
                 test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                 size_t mem_sizes[] = {897, 178, 790, 442, 483, 741, 333, 241, 658, 986, 765, 976, 127, 953, 940, 634, 571, 362, 339, 510, 380, 524, 187, 363};
                 size_t mem_units[] = {3, 4, 2, 5, 5, 4, 3, 1, 4, 4, 1, 5, 2, 5, 2, 4, 1, 2, 2, 4, 1, 2, 1, 1};
                 char *ptrs[24];

                 for (int i = 0; i < 24; ++i)
                 {
                     ptrs[i] = heap_calloc(mem_sizes[i], mem_units[i]);
                     test_error(ptrs[i] != NULL, "Funkcja heap_calloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                     test_error((intptr_t)ptrs[i] % sizeof(void *) == 0, "Funkcja heap_malloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");

                     status = heap_validate();
                     test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                 }

                 size_t block_size = heap_get_largest_used_block_size();
                 test_error(block_size == 4880, "Funkcja heap_get_largest_used_block_size() powinna zwrócić wartość 4880, a zwróciła na %lu", block_size);

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                
             
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 34: Sprawdzanie poprawności działania funkcji heap_get_largest_used_block_size
//
void UTEST34(void)
{
    // informacje o teście
    test_start(34, "Sprawdzanie poprawności działania funkcji heap_get_largest_used_block_size", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    rldebug_heap_set_global_limit(0);
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                size_t mem_sizes[] = {600, 189, 196, 901, 686, 785, 452, 625, 837, 343, 409, 131, 775, 690, 910, 648, 467, 452, 118, 334, 932, 480, 394, 791, 588, 255, 699, 609, 851, 696, 894, 417, 743, 768, 283, 807, 471, 634, 585, 470};
                char *ptrs[40];

                for (int i = 0; i < 40; ++i)
                {
                    ptrs[i] = heap_malloc(mem_sizes[i]);
                    test_error(pointer_valid == get_pointer_type(ptrs[i]), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptrs[i]));
                    test_error((intptr_t)ptrs[i] % sizeof(void *) == 0, "Funkcja heap_malloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");

                    status = heap_validate();
                    test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                }           

                ptrs[18][-1] += 7 ;

                status = heap_validate();
                test_error(status == 1, "Funkcja heap_validate() powinna zwrócić wartość 1, a zwróciła na %d", status);

                size_t block_size = heap_get_largest_used_block_size();
                test_error(block_size == 0, "Funkcja heap_get_largest_used_block_size() powinna zwrócić wartość 0, a zwróciła na %lu", block_size);

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 35: Sprawdzanie poprawności działania funkcji heap_free - test sprawdza poprawność działania funkcji w przypadku przekazania do niej niepoprawnego wskaźnika lub wskaźnika sprzed inicjalizacji sterty
//
void UTEST35(void)
{
    // informacje o teście
    test_start(35, "Sprawdzanie poprawności działania funkcji heap_free - test sprawdza poprawność działania funkcji w przypadku przekazania do niej niepoprawnego wskaźnika lub wskaźnika sprzed inicjalizacji sterty", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                char *ptr = (char *)(intptr_t)rand();
                
                heap_free(ptr);
                
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);
            
                heap_free(NULL);
                
                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                heap_free(ptr);

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                

            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 36: Sprawdzanie poprawności działania funkcji heap_free - test sprawdza poprawność działania funkcji w przypadku przekazania do niej niepoprawnego wskaźnika
//
void UTEST36(void)
{
    // informacje o teście
    test_start(36, "Sprawdzanie poprawności działania funkcji heap_free - test sprawdza poprawność działania funkcji w przypadku przekazania do niej niepoprawnego wskaźnika", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                char *ptr = heap_malloc(172);
                test_error(ptr != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr));
                test_error((intptr_t)ptr % sizeof(void *) == 0, "Funkcja heap_malloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                heap_free(ptr + 1);

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                test_error(heap_get_largest_used_block_size() == 172, "Funkcja heap_get_largest_used_block_size() powinna zwrócić wartość 172, a zwróciła na %llu", heap_get_largest_used_block_size());

                heap_free(ptr - 1);

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                test_error(heap_get_largest_used_block_size() == 172, "Funkcja heap_get_largest_used_block_size() powinna zwrócić wartość 172, a zwróciła na %llu", heap_get_largest_used_block_size());

                heap_free(ptr + 81);

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                test_error(heap_get_largest_used_block_size() == 172, "Funkcja heap_get_largest_used_block_size() powinna zwrócić wartość 172, a zwróciła na %llu", heap_get_largest_used_block_size());

                heap_free(ptr - 22);

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                test_error(heap_get_largest_used_block_size() == 172, "Funkcja heap_get_largest_used_block_size() powinna zwrócić wartość 172, a zwróciła na %llu", heap_get_largest_used_block_size());

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 37: Sprawdzanie poprawności działania funkcji heap_free - test sprawdza poprawność działania funkcji w przypadku przekazania do niej niepoprawnego wskaźnika
//
void UTEST37(void)
{
    // informacje o teście
    test_start(37, "Sprawdzanie poprawności działania funkcji heap_free - test sprawdza poprawność działania funkcji w przypadku przekazania do niej niepoprawnego wskaźnika", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr1 = heap_malloc(349);
                test_error(ptr1 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr1), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr1));
                test_error((intptr_t)ptr1 % sizeof(void *) == 0, "Funkcja heap_malloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");

                void *ptr2 = heap_malloc(74);
                test_error(ptr2 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr2), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr2));
                test_error((intptr_t)ptr2 % sizeof(void *) == 0, "Funkcja heap_malloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                test_error(heap_get_largest_used_block_size() == 349, "Funkcja heap_get_largest_used_block_size() powinna zwrócić wartość 349, a zwróciła na %llu", heap_get_largest_used_block_size());

                heap_free(ptr1);

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                test_error(heap_get_largest_used_block_size() == 74, "Funkcja heap_get_largest_used_block_size() powinna zwrócić wartość 74, a zwróciła na %llu", heap_get_largest_used_block_size());

                test_error(pointer_unallocated == get_pointer_type(ptr1), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_unallocated, a zwróciła %d", get_pointer_type(ptr1));

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 38: Sprawdzanie poprawności działania funkcji heap_free - test sprawdza poprawność działania funkcji w przypadku przekazania do niej niepoprawnego wskaźnika
//
void UTEST38(void)
{
    // informacje o teście
    test_start(38, "Sprawdzanie poprawności działania funkcji heap_free - test sprawdza poprawność działania funkcji w przypadku przekazania do niej niepoprawnego wskaźnika", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr1 = heap_malloc(358);
                test_error(ptr1 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr1), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr1));
                test_error((intptr_t)ptr1 % sizeof(void *) == 0, "Funkcja heap_malloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                test_error(heap_get_largest_used_block_size() == 358, "Funkcja heap_get_largest_used_block_size() powinna zwrócić wartość 358, a zwróciła na %llu", heap_get_largest_used_block_size());

                heap_free(ptr1);

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                test_error(heap_get_largest_used_block_size() == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %llu", heap_get_largest_used_block_size());

                test_error(pointer_unallocated == get_pointer_type(ptr1), "Funkcja get_pointer_type() powinna zwrócić wskaźnik typu pointer_unallocated, a zwróciła %d", get_pointer_type(ptr1));

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 39: Sprawdzanie poprawności działania funkcji heap_free - test sprawdza poprawność działania funkcji w przypadku przekazania do niej niepoprawnego wskaźnika
//
void UTEST39(void)
{
    // informacje o teście
    test_start(39, "Sprawdzanie poprawności działania funkcji heap_free - test sprawdza poprawność działania funkcji w przypadku przekazania do niej niepoprawnego wskaźnika", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr1 = heap_malloc(257);
                test_error(ptr1 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr1), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr1));
                test_error((intptr_t)ptr1 % sizeof(void *) == 0, "Funkcja heap_malloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");

                void *ptr2 = heap_malloc(58);
                test_error(ptr2 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr2), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr2));
                test_error((intptr_t)ptr2 % sizeof(void *) == 0, "Funkcja heap_malloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                test_error(heap_get_largest_used_block_size() == 257, "Funkcja heap_get_largest_used_block_size() powinna zwrócić wartość 257, a zwróciła na %llu", heap_get_largest_used_block_size());

                heap_free(ptr1);

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                test_error(heap_get_largest_used_block_size() == 58, "Funkcja heap_get_largest_used_block_size() powinna zwrócić wartość 58, a zwróciła na %llu", heap_get_largest_used_block_size());

                test_error(pointer_unallocated == get_pointer_type(ptr1), "Funkcja get_pointer_type() powinna zwrócić wskaźnik typu pointer_unallocated, a zwróciła %d", get_pointer_type(ptr1));

                heap_free(ptr2);

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                test_error(heap_get_largest_used_block_size() == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %llu", heap_get_largest_used_block_size());

                test_error(pointer_unallocated == get_pointer_type(ptr2), "Funkcja get_pointer_type() powinna zwrócić wskaźnik typu pointer_unallocated, a zwróciła %d", get_pointer_type(ptr2));


                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 40: Sprawdzanie poprawności działania funkcji heap_free - test sprawdza poprawność działania funkcji w przypadku zwolnienia dwóch bloków pamięci sąsiadujących ze sobą
//
void UTEST40(void)
{
    // informacje o teście
    test_start(40, "Sprawdzanie poprawności działania funkcji heap_free - test sprawdza poprawność działania funkcji w przypadku zwolnienia dwóch bloków pamięci sąsiadujących ze sobą", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr1 = heap_malloc(254);
                test_error(ptr1 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr1), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr1));
                test_error((intptr_t)ptr1 % sizeof(void *) == 0, "Funkcja heap_malloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");

                void *ptr2 = heap_malloc(630);
                test_error(ptr2 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr2), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr2));
                test_error((intptr_t)ptr2 % sizeof(void *) == 0, "Funkcja heap_malloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");

                void *ptr3 = heap_malloc(93);
                test_error(ptr3 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr3), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr3));
                test_error((intptr_t)ptr3 % sizeof(void *) == 0, "Funkcja heap_malloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");


                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                test_error(heap_get_largest_used_block_size() == 630, "Funkcja heap_get_largest_used_block_size() powinna zwrócić wartość 630, a zwróciła na %llu", heap_get_largest_used_block_size());

                heap_free(ptr2);

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                test_error(heap_get_largest_used_block_size() == 254, "Funkcja heap_get_largest_used_block_size() powinna zwrócić wartość 254, a zwróciła na %llu", heap_get_largest_used_block_size());

                test_error(pointer_unallocated == get_pointer_type(ptr2), "Funkcja get_pointer_type() powinna zwrócić wskaźnik typu pointer_unallocated, a zwróciła %d", get_pointer_type(ptr2));

                heap_free(ptr1);

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                test_error(heap_get_largest_used_block_size() == 93, "Funkcja heap_get_largest_used_block_size() powinna zwrócić wartość 93, a zwróciła na %llu", heap_get_largest_used_block_size());

                test_error(pointer_unallocated == get_pointer_type(ptr1), "Funkcja get_pointer_type() powinna zwrócić wskaźnik typu pointer_unallocated, a zwróciła %d", get_pointer_type(ptr1));

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 41: Sprawdzanie poprawności działania funkcji heap_free - test sprawdza poprawność działania funkcji w przypadku zwolnienia dwóch bloków pamięci sąsiadujących ze sobą i zaalokowanie w tym miejscu nowej pamięci o rozmiarze sumy dwóch zwolnionych bloków
//
void UTEST41(void)
{
    // informacje o teście
    test_start(41, "Sprawdzanie poprawności działania funkcji heap_free - test sprawdza poprawność działania funkcji w przypadku zwolnienia dwóch bloków pamięci sąsiadujących ze sobą i zaalokowanie w tym miejscu nowej pamięci o rozmiarze sumy dwóch zwolnionych bloków", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr1 = heap_malloc(378);
                test_error(ptr1 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr1), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr1));
                test_error((intptr_t)ptr1 % sizeof(void *) == 0, "Funkcja heap_malloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");

                void *ptr2 = heap_malloc(800);
                test_error(ptr2 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr2), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr2));
                test_error((intptr_t)ptr2 % sizeof(void *) == 0, "Funkcja heap_malloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");

                void *ptr3 = heap_malloc(68);
                test_error(ptr3 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr3), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr3));
                test_error((intptr_t)ptr3 % sizeof(void *) == 0, "Funkcja heap_malloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");


                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                test_error(heap_get_largest_used_block_size() == 800, "Funkcja heap_get_largest_used_block_size() powinna zwrócić wartość 800, a zwróciła na %llu", heap_get_largest_used_block_size());

                heap_free(ptr2);

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                test_error(heap_get_largest_used_block_size() == 378, "Funkcja heap_get_largest_used_block_size() powinna zwrócić wartość 378, a zwróciła na %llu", heap_get_largest_used_block_size());

                test_error(pointer_unallocated == get_pointer_type(ptr2), "Funkcja get_pointer_type() powinna zwrócić wskaźnik typu pointer_unallocated, a zwróciła %d", get_pointer_type(ptr2));

                heap_free(ptr1);

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                test_error(heap_get_largest_used_block_size() == 68, "Funkcja heap_get_largest_used_block_size() powinna zwrócić wartość 68, a zwróciła na %llu", heap_get_largest_used_block_size());

                test_error(pointer_unallocated == get_pointer_type(ptr1), "Funkcja get_pointer_type() powinna zwrócić wskaźnik typu pointer_unallocated, a zwróciła %d", get_pointer_type(ptr1));

                void *ptr4 = heap_malloc(1186);
                test_error(ptr4 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr4), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr4));
                test_error(ptr1 == ptr4, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego pierwszego bloku pamięci");
                test_error(heap_get_largest_used_block_size() == 1186, "Funkcja heap_validate() powinna zwrócić wartość 1186, a zwróciła na %llu", heap_get_largest_used_block_size());

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 42: Sprawdzanie poprawności działania funkcji heap_free - test sprawdza poprawność działania funkcji w przypadku zwolnienia dwóch bloków pamięci sąsiadujących ze sobą
//
void UTEST42(void)
{
    // informacje o teście
    test_start(42, "Sprawdzanie poprawności działania funkcji heap_free - test sprawdza poprawność działania funkcji w przypadku zwolnienia dwóch bloków pamięci sąsiadujących ze sobą", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr1 = heap_malloc(383);
                test_error(ptr1 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr1), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr1));
                test_error((intptr_t)ptr1 % sizeof(void *) == 0, "Funkcja heap_malloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");

                void *ptr2 = heap_malloc(9261);
                test_error(ptr2 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr2), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr2));
                test_error((intptr_t)ptr2 % sizeof(void *) == 0, "Funkcja heap_malloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");

                void *ptr3 = heap_malloc(957);
                test_error(ptr3 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr3), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr3));
                test_error((intptr_t)ptr3 % sizeof(void *) == 0, "Funkcja heap_malloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");

                void *ptr4 = heap_malloc(68);
                test_error(ptr4 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr4), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr4));
                test_error((intptr_t)ptr4 % sizeof(void *) == 0, "Funkcja heap_malloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                test_error(heap_get_largest_used_block_size() == 9261, "Funkcja heap_get_largest_used_block_size() powinna zwrócić wartość 9261, a zwróciła na %llu", heap_get_largest_used_block_size());

                heap_free(ptr2);

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                test_error(heap_get_largest_used_block_size() == 957, "Funkcja heap_get_largest_used_block_size() powinna zwrócić wartość 957, a zwróciła na %llu", heap_get_largest_used_block_size());

                test_error(pointer_unallocated == get_pointer_type(ptr2), "Funkcja get_pointer_type() powinna zwrócić wskaźnik typu pointer_unallocated, a zwróciła %d", get_pointer_type(ptr2));

                heap_free(ptr3);

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                test_error(heap_get_largest_used_block_size() == 383, "Funkcja heap_get_largest_used_block_size() powinna zwrócić wartość 383, a zwróciła na %llu", heap_get_largest_used_block_size());

                test_error(pointer_unallocated == get_pointer_type(ptr3), "Funkcja get_pointer_type() powinna zwrócić wskaźnik typu pointer_unallocated, a zwróciła %d", get_pointer_type(ptr3));

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 43: Sprawdzanie poprawności działania funkcji heap_free - test sprawdza poprawność działania funkcji w przypadku zwolnienia dwóch bloków pamięci sąsiadujących ze sobą i zaalokowanie w tym miejscu nowej pamięci o rozmiarze sumy dwóch zwolnionych bloków
//
void UTEST43(void)
{
    // informacje o teście
    test_start(43, "Sprawdzanie poprawności działania funkcji heap_free - test sprawdza poprawność działania funkcji w przypadku zwolnienia dwóch bloków pamięci sąsiadujących ze sobą i zaalokowanie w tym miejscu nowej pamięci o rozmiarze sumy dwóch zwolnionych bloków", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr1 = heap_malloc(378);
                test_error(ptr1 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr1), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr1));
                test_error((intptr_t)ptr1 % sizeof(void *) == 0, "Funkcja heap_malloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");

                void *ptr2 = heap_malloc(6025);
                test_error(ptr2 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr2), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr2));
                test_error((intptr_t)ptr2 % sizeof(void *) == 0, "Funkcja heap_malloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");

                void *ptr3 = heap_malloc(695);
                test_error(ptr3 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr3), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr3));
                test_error((intptr_t)ptr3 % sizeof(void *) == 0, "Funkcja heap_malloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");

                void *ptr4 = heap_malloc(58);
                test_error(ptr4 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr4), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr4));
                test_error((intptr_t)ptr4 % sizeof(void *) == 0, "Funkcja heap_malloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                test_error(heap_get_largest_used_block_size() == 6025, "Funkcja heap_get_largest_used_block_size() powinna zwrócić wartość 6025, a zwróciła na %llu", heap_get_largest_used_block_size());

                heap_free(ptr2);

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                test_error(heap_get_largest_used_block_size() == 695, "Funkcja heap_get_largest_used_block_size() powinna zwrócić wartość 695, a zwróciła na %llu", heap_get_largest_used_block_size());

                test_error(pointer_unallocated == get_pointer_type(ptr2), "Funkcja get_pointer_type() powinna zwrócić wskaźnik typu pointer_unallocated, a zwróciła %d", get_pointer_type(ptr2));

                heap_free(ptr3);

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                test_error(heap_get_largest_used_block_size() == 378, "Funkcja heap_get_largest_used_block_size() powinna zwrócić wartość 378, a zwróciła na %llu", heap_get_largest_used_block_size());

                test_error(pointer_unallocated == get_pointer_type(ptr3), "Funkcja get_pointer_type() powinna zwrócić wskaźnik typu pointer_unallocated, a zwróciła %d", get_pointer_type(ptr3));

                void *ptr = heap_malloc(6728);
                test_error(ptr != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr));
                test_error(ptr2 == ptr, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego pierwszego bloku pamięci");
                test_error(heap_get_largest_used_block_size() == 6728, "Funkcja heap_validate() powinna zwrócić wartość 6728, a zwróciła na %llu", heap_get_largest_used_block_size());


                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 44: Sprawdzanie poprawności działania funkcji heap_free - test sprawdza poprawność działania funkcji w przypadku zwolnienia trzech bloków pamięci sąsiadujących ze sobą
//
void UTEST44(void)
{
    // informacje o teście
    test_start(44, "Sprawdzanie poprawności działania funkcji heap_free - test sprawdza poprawność działania funkcji w przypadku zwolnienia trzech bloków pamięci sąsiadujących ze sobą", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr1 = heap_malloc(7791);
                test_error(ptr1 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr1), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr1));
                test_error((intptr_t)ptr1 % sizeof(void *) == 0, "Funkcja heap_malloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");

                void *ptr2 = heap_malloc(315);
                test_error(ptr2 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr2), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr2));
                test_error((intptr_t)ptr2 % sizeof(void *) == 0, "Funkcja heap_malloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");

                void *ptr3 = heap_malloc(782);
                test_error(ptr3 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr3), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr3));
                test_error((intptr_t)ptr3 % sizeof(void *) == 0, "Funkcja heap_malloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");

                void *ptr4 = heap_malloc(88);
                test_error(ptr4 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr4), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr4));
                test_error((intptr_t)ptr4 % sizeof(void *) == 0, "Funkcja heap_malloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                test_error(heap_get_largest_used_block_size() == 7791, "Funkcja heap_get_largest_used_block_size() powinna zwrócić wartość 7791, a zwróciła na %llu", heap_get_largest_used_block_size());

                heap_free(ptr1);

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                test_error(heap_get_largest_used_block_size() == 782, "Funkcja heap_get_largest_used_block_size() powinna zwrócić wartość 782, a zwróciła na %llu", heap_get_largest_used_block_size());

                test_error(pointer_unallocated == get_pointer_type(ptr1), "Funkcja get_pointer_type() powinna zwrócić wskaźnik typu pointer_unallocated, a zwróciła %d", get_pointer_type(ptr1));

                heap_free(ptr3);

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                test_error(heap_get_largest_used_block_size() == 315, "Funkcja heap_get_largest_used_block_size() powinna zwrócić wartość 315, a zwróciła na %llu", heap_get_largest_used_block_size());
                test_error(pointer_unallocated == get_pointer_type(ptr3), "Funkcja get_pointer_type() powinna zwrócić wskaźnik typu pointer_unallocated, a zwróciła %d", get_pointer_type(ptr3));

                heap_free(ptr2);

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                test_error(heap_get_largest_used_block_size() == 88, "Funkcja heap_get_largest_used_block_size() powinna zwrócić wartość 88, a zwróciła na %llu", heap_get_largest_used_block_size());
                test_error(pointer_unallocated == get_pointer_type(ptr3), "Funkcja get_pointer_type() powinna zwrócić wskaźnik typu pointer_unallocated, a zwróciła %d", get_pointer_type(ptr3));


                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 45: Sprawdzanie poprawności działania funkcji heap_free - test sprawdza poprawność działania funkcji w przypadku zwolnienia trzech bloków pamięci sąsiadujących ze sobą i zaalokowanie w tym miejscu nowej pamięci o rozmiarze sumy dwóch zwolnionych bloków
//
void UTEST45(void)
{
    // informacje o teście
    test_start(45, "Sprawdzanie poprawności działania funkcji heap_free - test sprawdza poprawność działania funkcji w przypadku zwolnienia trzech bloków pamięci sąsiadujących ze sobą i zaalokowanie w tym miejscu nowej pamięci o rozmiarze sumy dwóch zwolnionych bloków", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr1 = heap_malloc(6864);
                test_error(ptr1 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr1), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr1));
                test_error((intptr_t)ptr1 % sizeof(void *) == 0, "Funkcja heap_malloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");

                void *ptr2 = heap_malloc(396);
                test_error(ptr2 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr2), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr2));
                test_error((intptr_t)ptr2 % sizeof(void *) == 0, "Funkcja heap_malloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");

                void *ptr3 = heap_malloc(945);
                test_error(ptr3 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr3), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr3));
                test_error((intptr_t)ptr3 % sizeof(void *) == 0, "Funkcja heap_malloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");

                void *ptr4 = heap_malloc(81);
                test_error(ptr4 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr4), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr4));
                test_error((intptr_t)ptr4 % sizeof(void *) == 0, "Funkcja heap_malloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                test_error(heap_get_largest_used_block_size() == 6864, "Funkcja heap_validate() powinna zwrócić wartość 6864, a zwróciła na %llu", heap_get_largest_used_block_size());

                heap_free(ptr1);

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                test_error(heap_get_largest_used_block_size() == 945, "Funkcja heap_validate() powinna zwrócić wartość 945, a zwróciła na %llu", heap_get_largest_used_block_size());

                test_error(pointer_unallocated == get_pointer_type(ptr1), "Funkcja get_pointer_type() powinna zwrócić wskaźnik typu pointer_unallocated, a zwróciła %d", get_pointer_type(ptr1));

                heap_free(ptr3);

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                test_error(heap_get_largest_used_block_size() == 396, "Funkcja heap_validate() powinna zwrócić wartość 396, a zwróciła na %llu", heap_get_largest_used_block_size());
                test_error(pointer_unallocated == get_pointer_type(ptr3), "Funkcja get_pointer_type() powinna zwrócić wskaźnik typu pointer_unallocated, a zwróciła %d", get_pointer_type(ptr3));

                heap_free(ptr2);

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                test_error(heap_get_largest_used_block_size() == 81, "Funkcja heap_validate() powinna zwrócić wartość 81, a zwróciła na %llu", heap_get_largest_used_block_size());
                test_error(pointer_unallocated == get_pointer_type(ptr3), "Funkcja get_pointer_type() powinna zwrócić wskaźnik typu pointer_unallocated, a zwróciła %d", get_pointer_type(ptr3));

                void *ptr = heap_malloc(8216);
                test_error(ptr != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr));
                test_error(ptr1 == ptr, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego pierwszego bloku pamięci");
                test_error(heap_get_largest_used_block_size() == 8216, "Funkcja heap_validate() powinna zwrócić wartość 8216, a zwróciła na %llu", heap_get_largest_used_block_size());

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 46: Sprawdzanie poprawności działania funkcji heap_free - test sprawdza poprawność działania funkcji w przypadku zwolnienia bloku pamięci, zaalokowania w to miejsce mniejszego bloku, zwolnienia go i zaalokowania ponownie większego bloku
//
void UTEST46(void)
{
    // informacje o teście
    test_start(46, "Sprawdzanie poprawności działania funkcji heap_free - test sprawdza poprawność działania funkcji w przypadku zwolnienia bloku pamięci, zaalokowania w to miejsce mniejszego bloku, zwolnienia go i zaalokowania ponownie większego bloku", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr1 = heap_malloc(360);
                test_error(ptr1 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr1), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr1));
                test_error((intptr_t)ptr1 % sizeof(void *) == 0, "Funkcja heap_malloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");

                void *ptr2 = heap_malloc(97);
                test_error(ptr2 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr2), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr2));
                test_error((intptr_t)ptr2 % sizeof(void *) == 0, "Funkcja heap_malloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                test_error(heap_get_largest_used_block_size() == 360, "Funkcja heap_get_largest_used_block_size() powinna zwrócić wartość 360, a zwróciła na %llu", heap_get_largest_used_block_size());

                heap_free(ptr1);

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                test_error(heap_get_largest_used_block_size() == 97, "Funkcja heap_get_largest_used_block_size() powinna zwrócić wartość 97, a zwróciła na %llu", heap_get_largest_used_block_size());

                test_error(pointer_unallocated == get_pointer_type(ptr1), "Funkcja get_pointer_type() powinna zwrócić wskaźnik typu pointer_unallocated, a zwróciła %d", get_pointer_type(ptr1));

                void *ptr3 = heap_malloc(323);
                test_error(ptr3 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(ptr3 == ptr1, "Funkcja heap_malloc() powinna zwrócić adres pierwszego wolnego bloku pamięci");
                test_error(pointer_valid == get_pointer_type(ptr3), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr3));

                test_error(heap_get_largest_used_block_size() == 323, "Funkcja heap_get_largest_used_block_size() powinna zwrócić wartość 323, a zwróciła na %llu", heap_get_largest_used_block_size());

                heap_free(ptr3);

                void *ptr4 = heap_malloc(360);
                test_error(ptr4 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(ptr4 == ptr1, "Funkcja heap_malloc() powinna zwrócić adres pierwszego wolnego bloku pamięci");
                test_error(pointer_valid == get_pointer_type(ptr4), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr4));

                test_error(heap_get_largest_used_block_size() == 360, "Funkcja heap_get_largest_used_block_size() powinna zwrócić wartość 360, a zwróciła na %llu", heap_get_largest_used_block_size());


                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 47: Sprawdzanie poprawności działania funkcji heap_free - test sprawdza poprawność działania funkcji w przypadku zwolnienia trzech bloków pamięci sąsiadujących ze sobą i zaalokowanie w tym miejscu nowej pamięci o rozmiarze sumy dwóch zwolnionych bloków
//
void UTEST47(void)
{
    // informacje o teście
    test_start(47, "Sprawdzanie poprawności działania funkcji heap_free - test sprawdza poprawność działania funkcji w przypadku zwolnienia trzech bloków pamięci sąsiadujących ze sobą i zaalokowanie w tym miejscu nowej pamięci o rozmiarze sumy dwóch zwolnionych bloków", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr1 = heap_malloc(747);
                test_error(ptr1 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr1), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr1));
                test_error((intptr_t)ptr1 % sizeof(void *) == 0, "Funkcja heap_malloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");

                void *ptr2 = heap_malloc(368);
                test_error(ptr2 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr2), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr2));
                test_error((intptr_t)ptr2 % sizeof(void *) == 0, "Funkcja heap_malloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");

                void *ptr3 = heap_malloc(6538);
                test_error(ptr3 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr3), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr3));
                test_error((intptr_t)ptr3 % sizeof(void *) == 0, "Funkcja heap_malloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");

                void *ptr4 = heap_malloc(81);
                test_error(ptr4 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr4), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr4));
                test_error((intptr_t)ptr4 % sizeof(void *) == 0, "Funkcja heap_malloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                test_error(heap_get_largest_used_block_size() == 6538, "Funkcja heap_get_largest_used_block_size() powinna zwrócić wartość 6538, a zwróciła na %llu", heap_get_largest_used_block_size());

                heap_free(ptr3);

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                test_error(heap_get_largest_used_block_size() == 747, "Funkcja heap_get_largest_used_block_size() powinna zwrócić wartość 747, a zwróciła na %llu", heap_get_largest_used_block_size());

                test_error(pointer_unallocated == get_pointer_type(ptr3), "Funkcja get_pointer_type() powinna zwrócić wskaźnik typu pointer_unallocated, a zwróciła %d", get_pointer_type(ptr3));

                void *ptr5 = heap_malloc(4049);
                test_error(ptr5 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(ptr5 == ptr3, "Funkcja heap_malloc() powinna zwrócić adres pierwszego wolnego bloku pamięci");
                test_error(pointer_valid == get_pointer_type(ptr5), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr5));

                test_error(heap_get_largest_used_block_size() == 4049, "Funkcja heap_get_largest_used_block_size() powinna zwrócić wartość 4049, a zwróciła na %llu", heap_get_largest_used_block_size());

                heap_free(ptr5);
                heap_free(ptr2);
                
                void *ptr6 = heap_malloc(1127);
                test_error(ptr6 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(ptr6 == ptr2, "Funkcja heap_malloc() powinna zwrócić adres pierwszego wolnego bloku pamięci");
                test_error(pointer_valid == get_pointer_type(ptr6), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr6));

                test_error(heap_get_largest_used_block_size() == 1127, "Funkcja heap_get_largest_used_block_size() powinna zwrócić wartość 1127, a zwróciła na %llu", heap_get_largest_used_block_size());

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                

            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 48: Sprawdzanie poprawności działania funkcji heap_free - test sprawdza poprawność działania funkcji w przypadku zwolnienia trzech bloków pamięci sąsiadujących ze sobą i zaalokowanie w tym miejscu nowej pamięci o rozmiarze sumy dwóch zwolnionych bloków
//
void UTEST48(void)
{
    // informacje o teście
    test_start(48, "Sprawdzanie poprawności działania funkcji heap_free - test sprawdza poprawność działania funkcji w przypadku zwolnienia trzech bloków pamięci sąsiadujących ze sobą i zaalokowanie w tym miejscu nowej pamięci o rozmiarze sumy dwóch zwolnionych bloków", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr1 = heap_malloc(890);
                test_error(ptr1 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr1), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr1));
                test_error((intptr_t)ptr1 % sizeof(void *) == 0, "Funkcja heap_malloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");

                void *ptr2 = heap_malloc(345);
                test_error(ptr2 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr2), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr2));
                test_error((intptr_t)ptr2 % sizeof(void *) == 0, "Funkcja heap_malloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");

                void *ptr3 = heap_malloc(9788);
                test_error(ptr3 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr3), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr3));
                test_error((intptr_t)ptr3 % sizeof(void *) == 0, "Funkcja heap_malloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");

                void *ptr4 = heap_malloc(84);
                test_error(ptr4 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr4), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr4));
                test_error((intptr_t)ptr4 % sizeof(void *) == 0, "Funkcja heap_malloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                test_error(heap_get_largest_used_block_size() == 9788, "Funkcja heap_get_largest_used_block_size() powinna zwrócić wartość 9788, a zwróciła na %llu", heap_get_largest_used_block_size());

                heap_free(ptr3);

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                test_error(heap_get_largest_used_block_size() == 890, "Funkcja heap_get_largest_used_block_size() powinna zwrócić wartość 890, a zwróciła na %llu", heap_get_largest_used_block_size());

                test_error(pointer_unallocated == get_pointer_type(ptr3), "Funkcja get_pointer_type() powinna zwrócić wskaźnik typu pointer_unallocated, a zwróciła %d", get_pointer_type(ptr3));

                void *ptr5 = heap_malloc(7522);
                test_error(ptr5 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(ptr5 == ptr3, "Funkcja heap_malloc() powinna zwrócić adres pierwszego wolnego bloku pamięci");
                test_error(pointer_valid == get_pointer_type(ptr5), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr5));

                test_error(heap_get_largest_used_block_size() == 7522, "Funkcja heap_get_largest_used_block_size() powinna zwrócić wartość 7522, a zwróciła na %llu", heap_get_largest_used_block_size());

                heap_free(ptr2);
                heap_free(ptr5);

                void *ptr6 = heap_malloc(1248);
                test_error(ptr6 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(ptr6 == ptr2, "Funkcja heap_malloc() powinna zwrócić adres pierwszego wolnego bloku pamięci");
                test_error(pointer_valid == get_pointer_type(ptr6), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr6));

                test_error(heap_get_largest_used_block_size() == 1248, "Funkcja heap_get_largest_used_block_size() powinna zwrócić wartość 1248, a zwróciła na %llu", heap_get_largest_used_block_size());

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 49: Sprawdzanie poprawności działania funkcji heap_malloc, heap_calloc i heap_free - test sprawdza poprawność działania funkcji w przypadku zwolnienia trzech bloków pamięci sąsiadujących ze sobą i zaalokowanie w tym miejscu nowej pamięci o rozmiarze sumy dwóch zwolnionych bloków
//
void UTEST49(void)
{
    // informacje o teście
    test_start(49, "Sprawdzanie poprawności działania funkcji heap_malloc, heap_calloc i heap_free - test sprawdza poprawność działania funkcji w przypadku zwolnienia trzech bloków pamięci sąsiadujących ze sobą i zaalokowanie w tym miejscu nowej pamięci o rozmiarze sumy dwóch zwolnionych bloków", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr1 = heap_malloc(6046);
                test_error(ptr1 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr1), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr1));
                test_error((intptr_t)ptr1 % sizeof(void *) == 0, "Funkcja heap_malloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");

                void *ptr2 = heap_malloc(335);
                test_error(ptr2 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr2), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr2));
                test_error((intptr_t)ptr2 % sizeof(void *) == 0, "Funkcja heap_malloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");

                void *ptr3 = heap_calloc(565, 1);
                test_error(ptr3 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr3), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr3));
                test_error((intptr_t)ptr3 % sizeof(void *) == 0, "Funkcja heap_malloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");

                void *ptr4 = heap_malloc(61);
                test_error(ptr4 != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr4), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr4));
                test_error((intptr_t)ptr4 % sizeof(void *) == 0, "Funkcja heap_malloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                test_error(heap_get_largest_used_block_size() == 6046, "Funkcja heap_validate() powinna zwrócić wartość 6046, a zwróciła na %llu", heap_get_largest_used_block_size());

                heap_free(ptr1);

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                test_error(heap_get_largest_used_block_size() == 565, "Funkcja heap_validate() powinna zwrócić wartość 565, a zwróciła na %llu", heap_get_largest_used_block_size());

                test_error(pointer_unallocated == get_pointer_type(ptr1), "Funkcja get_pointer_type() powinna zwrócić wskaźnik typu pointer_unallocated, a zwróciła %d", get_pointer_type(ptr1));

                heap_free(ptr3);

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                test_error(heap_get_largest_used_block_size() == 335, "Funkcja heap_validate() powinna zwrócić wartość 335, a zwróciła na %llu", heap_get_largest_used_block_size());
                test_error(pointer_unallocated == get_pointer_type(ptr3), "Funkcja get_pointer_type() powinna zwrócić wskaźnik typu pointer_unallocated, a zwróciła %d", get_pointer_type(ptr3));

                heap_free(ptr2);

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                test_error(heap_get_largest_used_block_size() == 61, "Funkcja heap_validate() powinna zwrócić wartość 61, a zwróciła na %llu", heap_get_largest_used_block_size());
                test_error(pointer_unallocated == get_pointer_type(ptr3), "Funkcja get_pointer_type() powinna zwrócić wskaźnik typu pointer_unallocated, a zwróciła %d", get_pointer_type(ptr3));

                void *ptr = heap_malloc(6960);
                test_error(ptr != NULL, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr));
                test_error(ptr1 == ptr, "Funkcja heap_malloc() powinna zwrócić adres przydzielonego pierwszego bloku pamięci");
                test_error(heap_get_largest_used_block_size() == 6960, "Funkcja heap_validate() powinna zwrócić wartość 6960, a zwróciła na %llu", heap_get_largest_used_block_size());

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 50: Sprawdzanie poprawności działania funkcji get_pointer_type
//
void UTEST50(void)
{
    // informacje o teście
    test_start(50, "Sprawdzanie poprawności działania funkcji get_pointer_type", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    

                test_error(get_pointer_type(NULL) == pointer_null, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_null, a zwróciła %d", get_pointer_type(NULL));

            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 51: Sprawdzanie poprawności działania funkcji get_pointer_type
//
void UTEST51(void)
{
    // informacje o teście
    test_start(51, "Sprawdzanie poprawności działania funkcji get_pointer_type", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                test_error(get_pointer_type(NULL) == pointer_null, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_null, a zwróciła %d", get_pointer_type(NULL));

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                
        
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 52: Sprawdzanie poprawności działania funkcji get_pointer_type
//
void UTEST52(void)
{
    // informacje o teście
    test_start(52, "Sprawdzanie poprawności działania funkcji get_pointer_type", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr = heap_malloc(123);

                test_error(get_pointer_type(ptr) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr));
                test_error((intptr_t)ptr % sizeof(void *) == 0, "Funkcja heap_malloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 53: Sprawdzanie poprawności działania funkcji get_pointer_type
//
void UTEST53(void)
{
    // informacje o teście
    test_start(53, "Sprawdzanie poprawności działania funkcji get_pointer_type", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                char *ptr = heap_malloc(196);
                char *ptr1 = heap_malloc(169);

                test_error(get_pointer_type(ptr) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr));
                test_error(get_pointer_type(ptr1) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr1));

                test_error((intptr_t)ptr % sizeof(void *) == 0, "Funkcja heap_malloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");
                test_error((intptr_t)ptr1 % sizeof(void *) == 0, "Funkcja heap_malloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");

                heap_free(ptr);

                test_error(get_pointer_type(ptr) == pointer_unallocated, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_unallocated, a zwróciła %d", get_pointer_type(ptr));
                test_error(get_pointer_type(ptr + 14) == pointer_unallocated, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_unallocated, a zwróciła %d", get_pointer_type(ptr + 14));
                test_error(get_pointer_type(ptr + 196) == pointer_unallocated, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_unallocated, a zwróciła %d", get_pointer_type(ptr + 196));
                test_error(get_pointer_type(ptr - 1) == pointer_unallocated, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_unallocated, a zwróciła %d", get_pointer_type(ptr - 1));

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 54: Sprawdzanie poprawności działania funkcji get_pointer_type
//
void UTEST54(void)
{
    // informacje o teście
    test_start(54, "Sprawdzanie poprawności działania funkcji get_pointer_type", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                char *ptr = heap_malloc(139);
                char *ptr1 = heap_malloc(154);

                test_error(get_pointer_type(ptr) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr));
                test_error(get_pointer_type(ptr1) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr1));

                test_error((intptr_t)ptr % sizeof(void *) == 0, "Funkcja heap_malloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");
                test_error((intptr_t)ptr1 % sizeof(void *) == 0, "Funkcja heap_malloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");

                test_error(get_pointer_type(ptr + 139) == pointer_inside_fences, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_inside_fences, a zwróciła %d", get_pointer_type(ptr + 139));
                test_error(get_pointer_type(ptr - 1) == pointer_inside_fences, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_inside_fences, a zwróciła %d", get_pointer_type(ptr - 1));

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 55: Sprawdzanie poprawności działania funkcji get_pointer_type
//
void UTEST55(void)
{
    // informacje o teście
    test_start(55, "Sprawdzanie poprawności działania funkcji get_pointer_type", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                char *ptr = heap_malloc(174);

                test_error(get_pointer_type(ptr) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr));
                test_error((intptr_t)ptr % sizeof(void *) == 0, "Funkcja heap_malloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");

                for (int i = 1; i < 174; ++i)
                    test_error(get_pointer_type(ptr + i) == pointer_inside_data_block, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_inside_data_block, a zwróciła %d", get_pointer_type(ptr + i));

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 56: Sprawdzanie poprawności działania funkcji get_pointer_type
//
void UTEST56(void)
{
    // informacje o teście
    test_start(56, "Sprawdzanie poprawności działania funkcji get_pointer_type", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                char *ptr = heap_malloc(936);
                char *ptr1 = heap_malloc(1528);

                test_error(get_pointer_type(ptr1) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr1));
                test_error((intptr_t)ptr % sizeof(void *) == 0, "Funkcja heap_malloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");
                test_error((intptr_t)ptr1 % sizeof(void *) == 0, "Funkcja heap_malloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");

                int i = -1;

                while (get_pointer_type(ptr1 + i) == pointer_inside_fences) --i;

                test_error(get_pointer_type(ptr1 + i) == pointer_control_block, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_control_block, a zwróciła %d", get_pointer_type(ptr + i));

                while (get_pointer_type(ptr1 + i) == pointer_control_block) --i;

                test_error(get_pointer_type(ptr1 + i) == pointer_inside_fences, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_inside_fences, a zwróciła %d", get_pointer_type(ptr + i));

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 57: Sprawdzanie poprawności działania funkcji get_pointer_type
//
void UTEST57(void)
{
    // informacje o teście
    test_start(57, "Sprawdzanie poprawności działania funkcji get_pointer_type", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                char *ptr = heap_malloc(1600);
                char *ptr1 = heap_malloc(1496);

                test_error(get_pointer_type(ptr1) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr1));
                test_error((intptr_t)ptr % sizeof(void *) == 0, "Funkcja heap_malloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");
                test_error((intptr_t)ptr1 % sizeof(void *) == 0, "Funkcja heap_malloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");

                int i = 1600 + 1;

                while (get_pointer_type(ptr + i) == pointer_inside_fences) ++i;

                test_error(get_pointer_type(ptr + i) == pointer_control_block, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_control_block, a zwróciła %d", get_pointer_type(ptr + i));

                while (get_pointer_type(ptr + i) == pointer_control_block) ++i;

                test_error(get_pointer_type(ptr + i) == pointer_inside_fences, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_inside_fences, a zwróciła %d", get_pointer_type(ptr + i));

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 58: Sprawdzanie poprawności działania funkcji get_pointer_type
//
void UTEST58(void)
{
    // informacje o teście
    test_start(58, "Sprawdzanie poprawności działania funkcji get_pointer_type", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                char *ptr = heap_malloc(170);
                char *ptr1 = heap_malloc(116);

                test_error(get_pointer_type(ptr) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr));
                test_error(get_pointer_type(ptr1) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr1));

                test_error((intptr_t)ptr % sizeof(void *) == 0, "Funkcja heap_malloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");
                test_error((intptr_t)ptr1 % sizeof(void *) == 0, "Funkcja heap_malloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");

                int i = -1, fences_size_before = 0, fences_size_after = 0;

                while (get_pointer_type(ptr + i) == pointer_inside_fences) 
                {
                    fences_size_before++;
                    --i;
                }

                i = 170 + 1;

                while (get_pointer_type(ptr + i) == pointer_inside_fences)             
                {
                    fences_size_after++;
                    ++i;
                }

                heap_free(ptr);

                for (int i = -fences_size_before; i < 170 + fences_size_after; ++i)
                    test_error(get_pointer_type(ptr + i) == pointer_unallocated, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_unallocated, a zwróciła %d", get_pointer_type(ptr + i));

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                

            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 59: Sprawdzanie poprawności działania funkcji get_pointer_type
//
void UTEST59(void)
{
    // informacje o teście
    test_start(59, "Sprawdzanie poprawności działania funkcji get_pointer_type", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr = heap_calloc(193, 123);

                test_error(get_pointer_type(ptr) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr));

                test_error((intptr_t)ptr % sizeof(void *) == 0, "Funkcja heap_malloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                
        
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 60: Sprawdzanie poprawności działania funkcji get_pointer_type
//
void UTEST60(void)
{
    // informacje o teście
    test_start(60, "Sprawdzanie poprawności działania funkcji get_pointer_type", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                char *ptr = heap_calloc(195, 20);
                char *ptr1 = heap_calloc(180, 20);

                test_error(get_pointer_type(ptr) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr));
                test_error(get_pointer_type(ptr1) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr1));

                test_error((intptr_t)ptr % sizeof(void *) == 0, "Funkcja heap_malloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");
                test_error((intptr_t)ptr1 % sizeof(void *) == 0, "Funkcja heap_malloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");

                heap_free(ptr);

                test_error(get_pointer_type(ptr) == pointer_unallocated, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_unallocated, a zwróciła %d", get_pointer_type(ptr));
                test_error(get_pointer_type(ptr + 12) == pointer_unallocated, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_unallocated, a zwróciła %d", get_pointer_type(ptr + 12));
                test_error(get_pointer_type(ptr + 3900) == pointer_unallocated, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_unallocated, a zwróciła %d", get_pointer_type(ptr + 195));
                test_error(get_pointer_type(ptr - 1) == pointer_unallocated, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_unallocated, a zwróciła %d", get_pointer_type(ptr - 1));

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
                
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 61: Sprawdzanie poprawności działania funkcji get_pointer_type
//
void UTEST61(void)
{
    // informacje o teście
    test_start(61, "Sprawdzanie poprawności działania funkcji get_pointer_type", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                char *ptr = heap_calloc(111, 20);
                char *ptr1 = heap_calloc(177, 20);

                test_error(get_pointer_type(ptr) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr));
                test_error(get_pointer_type(ptr1) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr1));

                test_error((intptr_t)ptr % sizeof(void *) == 0, "Funkcja heap_malloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");
                test_error((intptr_t)ptr1 % sizeof(void *) == 0, "Funkcja heap_malloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");

                test_error(get_pointer_type(ptr + 2220) == pointer_inside_fences, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_inside_fences, a zwróciła %d", get_pointer_type(ptr + 111));
                test_error(get_pointer_type(ptr - 1) == pointer_inside_fences, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_inside_fences, a zwróciła %d", get_pointer_type(ptr - 1));

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);

            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 62: Sprawdzanie poprawności działania funkcji get_pointer_type
//
void UTEST62(void)
{
    // informacje o teście
    test_start(62, "Sprawdzanie poprawności działania funkcji get_pointer_type", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                char *ptr = heap_calloc(161, 20);

                test_error(get_pointer_type(ptr) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr));
                test_error((intptr_t)ptr % sizeof(void *) == 0, "Funkcja heap_malloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");

                for (int i = 1; i < 3220; ++i)
                    test_error(get_pointer_type(ptr + i) == pointer_inside_data_block, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_inside_data_block, a zwróciła %d", get_pointer_type(ptr + i));

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 63: Sprawdzanie poprawności działania funkcji get_pointer_type
//
void UTEST63(void)
{
    // informacje o teście
    test_start(63, "Sprawdzanie poprawności działania funkcji get_pointer_type", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                char *ptr = heap_calloc(1016, 18);
                char *ptr1 = heap_calloc(1048, 18);

                test_error(get_pointer_type(ptr1) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr1));

                test_error((intptr_t)ptr % sizeof(void *) == 0, "Funkcja heap_malloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");
                test_error((intptr_t)ptr1 % sizeof(void *) == 0, "Funkcja heap_malloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");

                int i = -1;

                while (get_pointer_type(ptr1 + i) == pointer_inside_fences) --i;

                test_error(get_pointer_type(ptr1 + i) == pointer_control_block, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_control_block, a zwróciła %d", get_pointer_type(ptr + i));

                while (get_pointer_type(ptr1 + i) == pointer_control_block) --i;

                test_error(get_pointer_type(ptr1 + i) == pointer_inside_fences, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_inside_fences, a zwróciła %d", get_pointer_type(ptr + i));

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 64: Sprawdzanie poprawności działania funkcji heap_validate
//
void UTEST64(void)
{
    // informacje o teście
    test_start(64, "Sprawdzanie poprawności działania funkcji heap_validate", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                int status = heap_validate();
                test_error(status == 2, "Funkcja heap_validate() powinna zwrócić wartość 2, a zwróciła na %d", status);
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 65: Sprawdzanie poprawności działania funkcji heap_validate
//
void UTEST65(void)
{
    // informacje o teście
    test_start(65, "Sprawdzanie poprawności działania funkcji heap_validate", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                char *ptr = heap_malloc(113);

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                heap_free(ptr);

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
     
                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 66: Sprawdzanie poprawności działania funkcji heap_validate
//
void UTEST66(void)
{
    // informacje o teście
    test_start(66, "Sprawdzanie poprawności działania funkcji heap_validate", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                char *ptr = heap_malloc(190);

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                int i = -1;

                while (get_pointer_type(ptr + i) == pointer_inside_fences)
                {
                    char temp = ptr[i];
                    ptr[i] += 19;

                    status = heap_validate();
                    test_error(status == 1, "Funkcja heap_validate() powinna zwrócić wartość 1, a zwróciła na %d", status);

                    ptr[i] = temp;

                    status = heap_validate();
                    test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                    --i;
                }

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 67: Sprawdzanie poprawności działania funkcji heap_validate
//
void UTEST67(void)
{
    // informacje o teście
    test_start(67, "Sprawdzanie poprawności działania funkcji heap_validate", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                char *ptr = heap_malloc(170);

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                int i = 170;

                while (get_pointer_type(ptr + i) == pointer_inside_fences)
                {
                    char temp = ptr[i];
                    ptr[i] += 17;

                    status = heap_validate();
                    test_error(status == 1, "Funkcja heap_validate() powinna zwrócić wartość 1, a zwróciła na %d", status);

                    ptr[i] = temp;

                    status = heap_validate();
                    test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                    ++i;
                }

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 68: Sprawdzanie poprawności działania funkcji heap_validate
//
void UTEST68(void)
{
    // informacje o teście
    test_start(68, "Sprawdzanie poprawności działania funkcji heap_validate", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                char *ptr = heap_malloc(162);
                char *ptr1 = heap_malloc(162);

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                test_error(get_pointer_type(ptr) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr));
                test_error(get_pointer_type(ptr1) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr1));

                int i = -1;

                while (get_pointer_type(ptr1 + i) == pointer_inside_fences) --i;

                while (get_pointer_type(ptr1 + i) == pointer_control_block)
                {
                    char temp = ptr1[i];
                    ptr1[i] += 17;

                    status = heap_validate();
                    test_error(status == 3, "Funkcja heap_validate() powinna zwrócić wartość 3, a zwróciła na %d", status);

                    ptr1[i] = temp;

                    status = heap_validate();
                    test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                    --i;
                }

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 69: Sprawdzanie poprawności działania funkcji heap_validate
//
void UTEST69(void)
{
    // informacje o teście
    test_start(69, "Sprawdzanie poprawności działania funkcji heap_validate", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    rldebug_heap_set_global_limit(0);
    
    //
    // -----------
    //
    
                 int status = heap_setup();
                 test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                 size_t mem_sizes[] = {913, 705, 641, 686, 306, 177, 808, 388, 167, 624, 622, 767, 638, 736, 994, 928, 910, 918, 487, 543, 471, 340, 719, 840, 805, 947, 133, 871, 650, 498, 643, 466, 372};
                 size_t mem_units[] = {1, 1, 1, 3, 1, 4, 1, 5, 2, 3, 5, 4, 1, 5, 3, 2, 2, 5, 3, 3, 4, 2, 1, 3, 2, 5, 3, 1, 3, 3, 1, 3, 5};
                 char *ptrs[33];

                 for (int i = 0; i < 33; ++i)
                     ptrs[i] = heap_calloc(mem_sizes[i], mem_units[i]);

                 ptrs[31][-1] += 6;
                 
                 status = heap_validate();
                 test_error(status == 1, "Funkcja heap_validate() powinna zwrócić wartość 1, a zwróciła na %d", status);

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
             
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 70: Sprawdzanie poprawności działania funkcji heap_validate
//
void UTEST70(void)
{
    // informacje o teście
    test_start(70, "Sprawdzanie poprawności działania funkcji heap_validate", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    rldebug_heap_set_global_limit(0);
    
    //
    // -----------
    //
    
                 int status = heap_setup();
                 test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                 size_t mem_sizes[] = {542, 649, 970, 567, 370, 564, 647, 443, 709, 810, 190, 615, 929, 872, 236, 329, 727, 285, 968, 129, 553, 911, 841, 177, 552, 175, 244, 608, 397, 888, 590};
                 size_t mem_units[] = {1, 2, 3, 2, 3, 3, 5, 1, 4, 1, 2, 2, 5, 1, 4, 4, 3, 5, 3, 5, 2, 5, 5, 4, 2, 1, 4, 2, 1, 5, 1};
                 char *ptrs[31];

                 for (int i = 0; i < 31; ++i)
                     ptrs[i] = heap_calloc(mem_sizes[i], mem_units[i]);

                 ptrs[24][mem_sizes[24] * mem_units[24]] += 9;

                 status = heap_validate();
                 test_error(status == 1, "Funkcja heap_validate() powinna zwrócić wartość 1, a zwróciła na %d", status);

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
             
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 71: Sprawdzanie poprawności działania funkcji heap_validate
//
void UTEST71(void)
{
    // informacje o teście
    test_start(71, "Sprawdzanie poprawności działania funkcji heap_validate", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    rldebug_heap_set_global_limit(0);
    
    //
    // -----------
    //
    
                 int status = heap_setup();
                 test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                 size_t mem_sizes[] = {568, 147, 754, 575, 100, 497, 370, 588, 492, 839, 987, 791, 783, 893, 496, 186, 883, 499, 505, 713, 335, 226, 396, 209, 513, 429, 278, 520, 732, 969, 709, 980, 321, 487, 301};
                 size_t mem_units[] = {3, 1, 1, 1, 4, 3, 2, 3, 3, 1, 3, 5, 3, 2, 2, 2, 1, 1, 2, 5, 5, 1, 2, 2, 5, 1, 5, 2, 5, 4, 4, 4, 4, 3, 2};
                 char *ptrs[35];

                 for (int i = 0; i < 35; ++i)
                     ptrs[i] = heap_calloc(mem_sizes[i], mem_units[i]);

                 int i = -1;

                 while (get_pointer_type(ptrs[13] + i) == pointer_inside_fences) --i;

                 ptrs[13][i - 27] += 7;

                 status = heap_validate();
                 test_error(status == 3, "Funkcja heap_validate() powinna zwrócić wartość 3, a zwróciła na %d", status);

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
             
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 72: Sprawdzanie poprawności działania funkcji heap_realloc - test sprawdza poprawność działania funkcji w przypadku przekazania do niej wartości 0
//
void UTEST72(void)
{
    // informacje o teście
    test_start(72, "Sprawdzanie poprawności działania funkcji heap_realloc - test sprawdza poprawność działania funkcji w przypadku przekazania do niej wartości 0", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr = heap_realloc(NULL, 0);

                test_error(ptr == NULL, "Funkcja heap_realloc() powinna zwrócić wartość NULL");

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 73: Sprawdzanie poprawności działania funkcji heap_realloc - test sprawdza poprawność działania funkcji w przypadku niezainicjowania sterty
//
void UTEST73(void)
{
    // informacje o teście
    test_start(73, "Sprawdzanie poprawności działania funkcji heap_realloc - test sprawdza poprawność działania funkcji w przypadku niezainicjowania sterty", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    

                void *ptr = heap_realloc(NULL, 157);

                test_error(ptr == NULL, "Funkcja heap_realloc() powinna zwrócić wartość NULL");
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 74: Sprawdzanie poprawności działania funkcji heap_realloc - test sprawdza poprawność działania funkcji w przypadku przekazania do niej poprawnej wartości
//
void UTEST74(void)
{
    // informacje o teście
    test_start(74, "Sprawdzanie poprawności działania funkcji heap_realloc - test sprawdza poprawność działania funkcji w przypadku przekazania do niej poprawnej wartości", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr = heap_realloc(NULL, 185);

                test_error(ptr != NULL, "Funkcja heap_realloc() powinna zwrócić adres pamięci przydzielonej użytkownikowi");
                test_error(get_pointer_type(ptr) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr));

                test_error((intptr_t)ptr % sizeof(void *) == 0, "Funkcja heap_realloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 75: Sprawdzanie poprawności działania funkcji heap_realloc - test sprawdza poprawność działania funkcji w przypadku przekazania do niej wartości 0
//
void UTEST75(void)
{
    // informacje o teście
    test_start(75, "Sprawdzanie poprawności działania funkcji heap_realloc - test sprawdza poprawność działania funkcji w przypadku przekazania do niej wartości 0", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr = heap_realloc(NULL, 191);

                test_error(ptr != NULL, "Funkcja heap_realloc() powinna zwrócić adres pamięci przydzielonej użytkownikowi");
                test_error(get_pointer_type(ptr) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr));

                test_error((intptr_t)ptr % sizeof(void *) == 0, "Funkcja heap_realloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");

                void *ptr1 = heap_realloc(ptr, 0);

                test_error(ptr1 == NULL, "Funkcja heap_realloc() powinna zwrócić wartość NULL");
                test_error(get_pointer_type(ptr) == pointer_unallocated, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_unallocated, a zwróciła %d", get_pointer_type(ptr));

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 76: Sprawdzanie poprawności działania funkcji heap_realloc - test sprawdza poprawność działania funkcji w przypadku przekazania do niej niewłaściwego wskaźnika
//
void UTEST76(void)
{
    // informacje o teście
    test_start(76, "Sprawdzanie poprawności działania funkcji heap_realloc - test sprawdza poprawność działania funkcji w przypadku przekazania do niej niewłaściwego wskaźnika", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                char *ptr = heap_realloc(NULL, 121);

                test_error(ptr != NULL, "Funkcja heap_realloc() powinna zwrócić adres pamięci przydzielonej użytkownikowi");
                test_error(get_pointer_type(ptr) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr));
                test_error((intptr_t)ptr % sizeof(void *) == 0, "Funkcja heap_realloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");

                void *ptr1 = heap_realloc(ptr + 61, 121);

                test_error(ptr1 == NULL, "Funkcja heap_realloc() powinna zwrócić wartość NULL");
                test_error(get_pointer_type(ptr) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr));
        
                ptr1 = heap_realloc(ptr - 39, 121);

                test_error(ptr1 == NULL, "Funkcja heap_realloc() powinna zwrócić wartość NULL");
                test_error(get_pointer_type(ptr) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr));

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 77: Sprawdzanie poprawności działania funkcji heap_realloc - test sprawdza poprawność działania funkcji w przypadku żądania zmniejszenia rozmiaru pamięci
//
void UTEST77(void)
{
    // informacje o teście
    test_start(77, "Sprawdzanie poprawności działania funkcji heap_realloc - test sprawdza poprawność działania funkcji w przypadku żądania zmniejszenia rozmiaru pamięci", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                unsigned char values[167];
                
                for (int i = 0; i < 167; ++i)
                    values[i] = rand() % 255;

                char *ptr = heap_realloc(NULL, 167);
                char *ptr2 = heap_realloc(NULL, 167);

                test_error(ptr != NULL, "Funkcja heap_realloc() powinna zwrócić adres pamięci przydzielonej użytkownikowi");
                test_error(get_pointer_type(ptr) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr));

                test_error(ptr2 != NULL, "Funkcja heap_realloc() powinna zwrócić adres pamięci przydzielonej użytkownikowi");
                test_error(get_pointer_type(ptr2) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr2));

                test_error((intptr_t)ptr % sizeof(void *) == 0, "Funkcja heap_realloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");
                test_error((intptr_t)ptr2 % sizeof(void *) == 0, "Funkcja heap_realloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");

                memcpy(ptr, values, 167);

                void *ptr1 = heap_realloc(ptr, 53);

                test_error(ptr1 == ptr, "Funkcja heap_realloc() powinna zwrócić ten sam adres, który został do niej przekazany");
                test_error(memcmp(ptr, values, 53) == 0, "Funkcja heap_realloc() nie powinna modyfikować obszaru pamięci użytkownika");

                test_error(get_pointer_type(ptr + 53) == pointer_inside_fences, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_inside_fences, a zwróciła %d", get_pointer_type(ptr + 53));

                 int i = 53;

                char temp = ptr[i];
                ptr[i] += 17;

                status = heap_validate();
                test_error(status == 1, "Funkcja heap_validate() powinna zwrócić wartość 1, a zwróciła na %d", status);

                ptr[i] = temp;

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                while (get_pointer_type(ptr + i) == pointer_inside_fences) ++i;

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 78: Sprawdzanie poprawności działania funkcji heap_realloc - test sprawdza poprawność działania funkcji w przypadku żądania tego samego rozmiaru pamięci, który zajmował poprzednio
//
void UTEST78(void)
{
    // informacje o teście
    test_start(78, "Sprawdzanie poprawności działania funkcji heap_realloc - test sprawdza poprawność działania funkcji w przypadku żądania tego samego rozmiaru pamięci, który zajmował poprzednio", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                 int status = heap_setup();
                 test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                 unsigned char values[190];

                 for (int i = 0; i < 190; ++i)
                     values[i] = rand() % 255;

                 char *ptr = heap_realloc(NULL, 190);
                 char *ptr2 = heap_realloc(NULL, 190);

                 test_error(ptr != NULL, "Funkcja heap_realloc() powinna zwrócić adres pamięci przydzielonej użytkownikowi");
                 test_error(get_pointer_type(ptr) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr));

                 test_error(ptr2 != NULL, "Funkcja heap_realloc() powinna zwrócić adres pamięci przydzielonej użytkownikowi");
                 test_error(get_pointer_type(ptr2) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr2));

                test_error((intptr_t)ptr % sizeof(void *) == 0, "Funkcja heap_realloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");
                test_error((intptr_t)ptr2 % sizeof(void *) == 0, "Funkcja heap_realloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");

                 memcpy(ptr, values, 190);

                 void *ptr1 = heap_realloc(ptr, 190);

                 test_error(ptr1 == ptr, "Funkcja heap_realloc() powinna zwrócić ten sam adres, który został do niej przekazany");
                 test_error(memcmp(ptr, values, 190) == 0, "Funkcja heap_realloc() nie powinna modyfikować obszaru pamięci użytkownika");

                 test_error(get_pointer_type(ptr + 190) == pointer_inside_fences, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_inside_fences, a zwróciła %d", get_pointer_type(ptr + 190));

                  int i = 190;

                 char temp = ptr[i];
                 ptr[i] += 17;

                 status = heap_validate();
                 test_error(status == 1, "Funkcja heap_validate() powinna zwrócić wartość 1, a zwróciła na %d", status);

                 ptr[i] = temp;

                 status = heap_validate();
                 test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);

             
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 79: Sprawdzanie poprawności działania funkcji heap_realloc - test sprawdza poprawność działania funkcji w przypadku żądania większego rozmiaru pamięci, który zajmował poprzednio
//
void UTEST79(void)
{
    // informacje o teście
    test_start(79, "Sprawdzanie poprawności działania funkcji heap_realloc - test sprawdza poprawność działania funkcji w przypadku żądania większego rozmiaru pamięci, który zajmował poprzednio", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                 int status = heap_setup();
                 test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                 unsigned char values[102];

                 for (int i = 0; i < 102; ++i)
                     values[i] = rand() % 255;

                 char *ptr = heap_realloc(NULL, 102);

                 test_error(ptr != NULL, "Funkcja heap_realloc() powinna zwrócić adres pamięci przydzielonej użytkownikowi");
                 test_error(get_pointer_type(ptr) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr));
                 test_error((intptr_t)ptr % sizeof(void *) == 0, "Funkcja heap_realloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");

                 memcpy(ptr, values, 102);

                 void *ptr1 = heap_realloc(ptr, 539);

                 test_error(ptr1 == ptr, "Funkcja heap_realloc() powinna zwrócić ten sam adres, który został do niej przekazany");
                 test_error(memcmp(ptr, values, 102) == 0, "Funkcja heap_realloc() nie powinna modyfikować obszaru pamięci użytkownika");

                 test_error(get_pointer_type(ptr + 539) == pointer_inside_fences, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_inside_fences, a zwróciła %d", get_pointer_type(ptr + 190));

                  int i = 539;

                 char temp = ptr[i];
                 ptr[i] += 17;

                 status = heap_validate();
                 test_error(status == 1, "Funkcja heap_validate() powinna zwrócić wartość 1, a zwróciła na %d", status);

                 ptr[i] = temp;

                 status = heap_validate();
                 test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
             
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 80: Sprawdzanie poprawności działania funkcji heap_realloc - test sprawdza poprawność działania funkcji w przypadku żądania większego rozmiaru pamięci, który zajmował poprzednio
//
void UTEST80(void)
{
    // informacje o teście
    test_start(80, "Sprawdzanie poprawności działania funkcji heap_realloc - test sprawdza poprawność działania funkcji w przypadku żądania większego rozmiaru pamięci, który zajmował poprzednio", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                 int status = heap_setup();
                 test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                 unsigned char values[111];

                 for (int i = 0; i < 111; ++i)
                     values[i] = rand() % 255;

                 char *ptr = heap_realloc(NULL, 111);

                 test_error(ptr != NULL, "Funkcja heap_realloc() powinna zwrócić adres pamięci przydzielonej użytkownikowi");
                 test_error(get_pointer_type(ptr) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr));
                 test_error((intptr_t)ptr % sizeof(void *) == 0, "Funkcja heap_realloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");

                 memcpy(ptr, values, 111);

                 void *ptr1 = heap_realloc(ptr, 16025136);

                 test_error(ptr1 == ptr, "Funkcja heap_realloc() powinna zwrócić ten sam adres, który został do niej przekazany");
                 test_error(memcmp(ptr, values, 111) == 0, "Funkcja heap_realloc() nie powinna modyfikować obszaru pamięci użytkownika");

                 test_error(get_pointer_type(ptr + 16025136) == pointer_inside_fences, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_inside_fences, a zwróciła %d", get_pointer_type(ptr + 190));

                  int i = 16025136;

                 char temp = ptr[i];
                 ptr[i] += 17;

                 status = heap_validate();
                 test_error(status == 1, "Funkcja heap_validate() powinna zwrócić wartość 1, a zwróciła na %d", status);

                 ptr[i] = temp;

                 status = heap_validate();
                 test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
             
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 81: Sprawdzanie poprawności działania funkcji heap_realloc - test sprawdza poprawność działania funkcji w przypadku żądania większego rozmiaru pamięci, który zajmował poprzednio
//
void UTEST81(void)
{
    // informacje o teście
    test_start(81, "Sprawdzanie poprawności działania funkcji heap_realloc - test sprawdza poprawność działania funkcji w przypadku żądania większego rozmiaru pamięci, który zajmował poprzednio", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                  int status = heap_setup();
                  test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                  unsigned char values[149];

                  for (int i = 0; i < 149; ++i)
                      values[i] = rand() % 255;

                  char *ptr1 = heap_realloc(NULL, 149);
                  char *ptr2 = heap_realloc(NULL, 1688);
                  char *ptr3 = heap_realloc(NULL, 146);
                  char *ptr4 = heap_realloc(NULL, 108);

                  test_error(get_pointer_type(ptr1) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr1));
                  test_error(get_pointer_type(ptr2) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr2));
                  test_error(get_pointer_type(ptr3) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr3));
                  test_error(get_pointer_type(ptr4) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr4));

                  test_error((intptr_t)ptr1 % sizeof(void *) == 0, "Funkcja heap_realloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");
                  test_error((intptr_t)ptr2 % sizeof(void *) == 0, "Funkcja heap_realloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");
                  test_error((intptr_t)ptr3 % sizeof(void *) == 0, "Funkcja heap_realloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");
                  test_error((intptr_t)ptr4 % sizeof(void *) == 0, "Funkcja heap_realloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");

                  heap_free(ptr2);
                
                  ptr2 = heap_realloc(NULL, 149);
                
                  heap_free(ptr3);

                  memcpy(ptr2, values, 149);

                  char *ptr5 = heap_realloc(ptr2, 1847);

                  test_error(ptr5 == ptr2, "Funkcja heap_realloc() powinna zwrócić ten sam adres, który został do niej przekazany");
                  test_error(memcmp(ptr2, values, 149) == 0, "Funkcja heap_realloc() nie powinna modyfikować obszaru pamięci użytkownika");

                  test_error(get_pointer_type(ptr2 + 1847) == pointer_inside_fences, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_inside_fences, a zwróciła %d", get_pointer_type(ptr2 + 1847));

                  status = heap_validate();
                  test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
              
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 82: Sprawdzanie poprawności działania funkcji heap_realloc - test sprawdza poprawność działania funkcji w przypadku żądania większego rozmiaru pamięci, który zajmował poprzednio
//
void UTEST82(void)
{
    // informacje o teście
    test_start(82, "Sprawdzanie poprawności działania funkcji heap_realloc - test sprawdza poprawność działania funkcji w przypadku żądania większego rozmiaru pamięci, który zajmował poprzednio", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                  int status = heap_setup();
                  test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                  unsigned char values[196];

                  for (int i = 0; i < 196; ++i)
                      values[i] = rand() % 255;

                  char *ptr1 = heap_realloc(NULL, 196);
                  char *ptr2 = heap_realloc(NULL, 1734);
                  char *ptr3 = heap_realloc(NULL, 152);
                  char *ptr4 = heap_realloc(NULL, 186);

                  test_error(get_pointer_type(ptr1) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr1));
                  test_error(get_pointer_type(ptr2) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr2));
                  test_error(get_pointer_type(ptr3) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr3));
                  test_error(get_pointer_type(ptr4) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr4));

                  test_error((intptr_t)ptr1 % sizeof(void *) == 0, "Funkcja heap_realloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");
                  test_error((intptr_t)ptr2 % sizeof(void *) == 0, "Funkcja heap_realloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");
                  test_error((intptr_t)ptr3 % sizeof(void *) == 0, "Funkcja heap_realloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");
                  test_error((intptr_t)ptr4 % sizeof(void *) == 0, "Funkcja heap_realloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");

                  heap_free(ptr2);

                  ptr2 = heap_realloc(NULL, 196);

                  heap_free(ptr3);

                  memcpy(ptr2, values, 196);

                  char *ptr5 = heap_realloc(ptr2, 1763);

                  test_error(ptr5 == ptr2, "Funkcja heap_realloc() powinna zwrócić ten sam adres, który został do niej przekazany");
                  test_error(memcmp(ptr2, values, 196) == 0, "Funkcja heap_realloc() nie powinna modyfikować obszaru pamięci użytkownika");

                  test_error(get_pointer_type(ptr2 + 1763) == pointer_inside_fences, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_inside_fences, a zwróciła %d", get_pointer_type(ptr2 + 1763));

                  status = heap_validate();
                  test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
              
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 83: Sprawdzanie poprawności działania funkcji heap_realloc - test sprawdza poprawność działania funkcji w przypadku żądania większego rozmiaru pamięci, który zajmował poprzednio
//
void UTEST83(void)
{
    // informacje o teście
    test_start(83, "Sprawdzanie poprawności działania funkcji heap_realloc - test sprawdza poprawność działania funkcji w przypadku żądania większego rozmiaru pamięci, który zajmował poprzednio", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                  int status = heap_setup();
                  test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                  unsigned char values[177];

                  for (int i = 0; i < 177; ++i)
                      values[i] = rand() % 255;

                  char *ptr1 = heap_realloc(NULL, 177);
                  char *ptr2 = heap_realloc(NULL, 1217);
                  char *ptr3 = heap_realloc(NULL, 120);
                  char *ptr4 = heap_realloc(NULL, 131);

                  test_error(get_pointer_type(ptr1) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr1));
                  test_error(get_pointer_type(ptr2) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr2));
                  test_error(get_pointer_type(ptr3) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr3));
                  test_error(get_pointer_type(ptr4) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr4));

                  test_error((intptr_t)ptr1 % sizeof(void *) == 0, "Funkcja heap_realloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");
                  test_error((intptr_t)ptr2 % sizeof(void *) == 0, "Funkcja heap_realloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");
                  test_error((intptr_t)ptr3 % sizeof(void *) == 0, "Funkcja heap_realloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");
                  test_error((intptr_t)ptr4 % sizeof(void *) == 0, "Funkcja heap_realloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");

                  heap_free(ptr2);

                  ptr2 = heap_realloc(NULL, 177);

                  heap_free(ptr3);

                  memcpy(ptr2, values, 177);

                  char *ptr5 = heap_realloc(ptr2, 3266);

                  test_error(ptr5 != ptr2, "Funkcja heap_realloc() powinna zwrócić ten sam adres, który został do niej przekazany");
                  test_error(memcmp(ptr5, values, 177) == 0, "Funkcja heap_realloc() nie powinna modyfikować obszaru pamięci użytkownika");

                  test_error(get_pointer_type(ptr5 + 3266) == pointer_inside_fences, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_inside_fences, a zwróciła %d", get_pointer_type(ptr5 + 3266));
                  test_error(get_pointer_type(ptr2) == pointer_unallocated, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_unallocated, a zwróciła %d", get_pointer_type(ptr2));
                  test_error((intptr_t)ptr5 % sizeof(void *) == 0, "Funkcja heap_malloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");

                  status = heap_validate();
                  test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
              
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 84: Sprawdzanie poprawności działania funkcji heap_realloc - test sprawdza poprawność działania funkcji w przypadku żądania większego rozmiaru pamięci, który zajmował poprzednio
//
void UTEST84(void)
{
    // informacje o teście
    test_start(84, "Sprawdzanie poprawności działania funkcji heap_realloc - test sprawdza poprawność działania funkcji w przypadku żądania większego rozmiaru pamięci, który zajmował poprzednio", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                  int status = heap_setup();
                  test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                  unsigned char values[126];

                  for (int i = 0; i < 126; ++i)
                      values[i] = rand() % 255;

                  char *ptr1 = heap_realloc(NULL, 126);
                  char *ptr2 = heap_realloc(NULL, 1580);
                  char *ptr3 = heap_realloc(NULL, 182);
                  char *ptr4 = heap_realloc(NULL, 161);

                  test_error(get_pointer_type(ptr1) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr1));
                  test_error(get_pointer_type(ptr2) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr2));
                  test_error(get_pointer_type(ptr3) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr3));
                  test_error(get_pointer_type(ptr4) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr4));

                  test_error((intptr_t)ptr1 % sizeof(void *) == 0, "Funkcja heap_realloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");
                  test_error((intptr_t)ptr2 % sizeof(void *) == 0, "Funkcja heap_realloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");
                  test_error((intptr_t)ptr3 % sizeof(void *) == 0, "Funkcja heap_realloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");
                  test_error((intptr_t)ptr4 % sizeof(void *) == 0, "Funkcja heap_realloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");

                  heap_free(ptr2);

                  ptr2 = heap_realloc(NULL, 126);

                  heap_free(ptr3);

                  memcpy(ptr2, values, 126);

                  char *ptr5 = heap_realloc(ptr2, 68039573);

                  test_error(ptr5 != ptr2, "Funkcja heap_realloc() powinna zwrócić ten sam adres, który został do niej przekazany");
                  test_error(memcmp(ptr2, values, 126) == 0, "Funkcja heap_realloc() nie powinna modyfikować obszaru pamięci użytkownika");

                  test_error(get_pointer_type(ptr2) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr2));
                  test_error(get_pointer_type(ptr5) == pointer_null, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_null, a zwróciła %d", get_pointer_type(ptr5));

                  test_error((intptr_t)ptr5 % sizeof(void *) == 0, "Funkcja heap_malloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");

                  status = heap_validate();
                  test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
              
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 85: Sprawdzanie poprawności działania funkcji heap_realloc - test sprawdza poprawność działania funkcji w przypadku żądania większego rozmiaru pamięci, który zajmował poprzednio
//
void UTEST85(void)
{
    // informacje o teście
    test_start(85, "Sprawdzanie poprawności działania funkcji heap_realloc - test sprawdza poprawność działania funkcji w przypadku żądania większego rozmiaru pamięci, który zajmował poprzednio", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                  int status = heap_setup();
                  test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                  unsigned char values[148];

                  for (int i = 0; i < 148; ++i)
                      values[i] = rand() % 255;

                  char *ptr1 = heap_realloc(NULL, 195);
                  char *ptr2 = heap_realloc(NULL, 1690);
                  char *ptr3 = heap_realloc(NULL, 106);
                  char *ptr4 = heap_realloc(NULL, 148);

                  test_error(get_pointer_type(ptr1) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr1));
                  test_error(get_pointer_type(ptr2) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr2));
                  test_error(get_pointer_type(ptr3) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr3));
                  test_error(get_pointer_type(ptr4) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr4));

                  test_error((intptr_t)ptr1 % sizeof(void *) == 0, "Funkcja heap_realloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");
                  test_error((intptr_t)ptr2 % sizeof(void *) == 0, "Funkcja heap_realloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");
                  test_error((intptr_t)ptr3 % sizeof(void *) == 0, "Funkcja heap_realloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");
                  test_error((intptr_t)ptr4 % sizeof(void *) == 0, "Funkcja heap_realloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");

                  memcpy(ptr4, values, 148);

                  char *ptr5 = heap_realloc(ptr4, 68061918);

                  test_error(ptr5 == NULL, "Funkcja heap_realloc() powinna zwrócić NULL");
                  test_error(memcmp(ptr4, values, 148) == 0, "Funkcja heap_realloc() nie powinna modyfikować obszaru pamięci użytkownika");

                  test_error(get_pointer_type(ptr4) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr2));
                  test_error(get_pointer_type(ptr5) == pointer_null, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_null, a zwróciła %d", get_pointer_type(ptr5));

                  test_error(get_pointer_type(ptr4 + 148) == pointer_inside_fences, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_inside_fences, a zwróciła %d", get_pointer_type(ptr4 + 148));

                  status = heap_validate();
                  test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
              
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 86: Sprawdzanie poprawności działania funkcji heap_realloc - test sprawdza poprawność działania funkcji w przypadku żądania większego rozmiaru pamięci, który zajmował poprzednio
//
void UTEST86(void)
{
    // informacje o teście
    test_start(86, "Sprawdzanie poprawności działania funkcji heap_realloc - test sprawdza poprawność działania funkcji w przypadku żądania większego rozmiaru pamięci, który zajmował poprzednio", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                  int status = heap_setup();
                  test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                  unsigned char values[116];

                  for (int i = 0; i < 116; ++i)
                      values[i] = rand() % 255;

                  char *ptr1 = heap_realloc(NULL, 116);
                  char *ptr2 = heap_realloc(NULL, 15107);
                  char *ptr3 = heap_realloc(NULL, 179);
                  char *ptr4 = heap_realloc(NULL, 126);

                  test_error(get_pointer_type(ptr1) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr1));
                  test_error(get_pointer_type(ptr2) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr2));
                  test_error(get_pointer_type(ptr3) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr3));
                  test_error(get_pointer_type(ptr4) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr4));

                  test_error((intptr_t)ptr1 % sizeof(void *) == 0, "Funkcja heap_realloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");
                  test_error((intptr_t)ptr2 % sizeof(void *) == 0, "Funkcja heap_realloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");
                  test_error((intptr_t)ptr3 % sizeof(void *) == 0, "Funkcja heap_realloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");
                  test_error((intptr_t)ptr4 % sizeof(void *) == 0, "Funkcja heap_realloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");

                  char *ptr5 = heap_realloc(ptr2, 116);
                  test_error(ptr5 == ptr2, "Funkcja heap_realloc() powinna zwrócić ten sam adres, który został do niej przekazany");

                  memcpy(ptr2, values, 116);

                  char *ptr6 = heap_realloc(NULL, 178);
                  test_error((intptr_t)ptr6 % sizeof(void *) == 0, "Funkcja heap_realloc() powinna zwrócić adres będący wielokrotnością słowa maszynowego");

                  test_error((intptr_t)ptr6 > (intptr_t)ptr2 && (intptr_t)ptr6 < (intptr_t)ptr3, "Funkcja heap_realloc() powinna zwrócić adres znajdujący się w zwolnionym przez poprzednie wywołanie funkcji heap_realloc bloku pamięci");


                  status = heap_validate();
                  test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
              
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 87: Sprawdzanie poprawności działania funkcji heap_malloc_aligned - test sprawdza poprawność działania funkcji w przypadku przekazania do niej wartości 0
//
void UTEST87(void)
{
    // informacje o teście
    test_start(87, "Sprawdzanie poprawności działania funkcji heap_malloc_aligned - test sprawdza poprawność działania funkcji w przypadku przekazania do niej wartości 0", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr = heap_malloc_aligned(0);

                test_error(ptr == NULL, "Funkcja heap_malloc_aligned() powinna zwrócić wartość NULL");

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 88: Sprawdzanie poprawności działania funkcji heap_malloc_aligned - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni większej niż przestrzeń obecna na stercie
//
void UTEST88(void)
{
    // informacje o teście
    test_start(88, "Sprawdzanie poprawności działania funkcji heap_malloc_aligned - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni większej niż przestrzeń obecna na stercie", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr = heap_malloc_aligned(67108839);

                test_error(ptr == NULL, "Funkcja heap_malloc_aligned() powinna zwrócić wartość NULL");

                heap_clean();
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 89: Sprawdzanie poprawności działania funkcji heap_malloc_aligned - test sprawdza poprawność działania funkcji w przypadku pierwszej próby zaalokowania przestrzeni mniejszej niż przestrzeń obecna na stercie
//
void UTEST89(void)
{
    // informacje o teście
    test_start(89, "Sprawdzanie poprawności działania funkcji heap_malloc_aligned - test sprawdza poprawność działania funkcji w przypadku pierwszej próby zaalokowania przestrzeni mniejszej niż przestrzeń obecna na stercie", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    rldebug_heap_set_global_limit(0);
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr = heap_malloc_aligned(306);

                test_error(ptr != NULL, "Funkcja heap_malloc_aligned() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr), "Funkcja heap_malloc_aligned() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr));
                test_error(((intptr_t)ptr & (intptr_t)(PAGE_SIZE - 1)) == 0, "Funkcja heap_malloc_aligned() powinien zwrócić adres zaczynający się na początku strony");
            
                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 90: Sprawdzanie poprawności działania funkcji heap_malloc_aligned - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni mniejszej niż przestrzeń obecna na stercie, w przypadku przydzielenia wcześniej pamięci użytkownikowi
//
void UTEST90(void)
{
    // informacje o teście
    test_start(90, "Sprawdzanie poprawności działania funkcji heap_malloc_aligned - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni mniejszej niż przestrzeń obecna na stercie, w przypadku przydzielenia wcześniej pamięci użytkownikowi", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    rldebug_heap_set_global_limit(0);
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr1 = heap_malloc_aligned(15);
                test_error(ptr1 != NULL, "Funkcja heap_malloc_aligned() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr1), "Funkcja heap_malloc_aligned() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr1));
                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                test_error(((intptr_t)ptr1 & (intptr_t)(PAGE_SIZE - 1)) == 0, "Funkcja heap_malloc_aligned() powinien zwrócić adres zaczynający się na początku strony");

                void *ptr2 = heap_malloc_aligned(200);
                test_error(ptr2 != NULL, "Funkcja heap_malloc_aligned() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr2), "Funkcja heap_malloc_aligned() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr2));
                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                test_error(((intptr_t)ptr2 & (intptr_t)(PAGE_SIZE - 1)) == 0, "Funkcja heap_malloc_aligned() powinien zwrócić adres zaczynający się na początku strony");

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 91: Sprawdzanie poprawności działania funkcji heap_malloc_aligned - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni mniejszej niż przestrzeń obecna na stercie (żądana pamięć wymaga zwiększenia limitu pamięci przydzielonego przez system), w przypadku przydzielenia wcześniej pamięci użytkownikowi
//
void UTEST91(void)
{
    // informacje o teście
    test_start(91, "Sprawdzanie poprawności działania funkcji heap_malloc_aligned - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni mniejszej niż przestrzeń obecna na stercie (żądana pamięć wymaga zwiększenia limitu pamięci przydzielonego przez system), w przypadku przydzielenia wcześniej pamięci użytkownikowi", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    rldebug_heap_set_global_limit(0);
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr1 = heap_malloc_aligned(62);
                test_error(ptr1 != NULL, "Funkcja heap_malloc_aligned() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr1), "Funkcja heap_malloc_aligned() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr1));
                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                test_error(((intptr_t)ptr1 & (intptr_t)(PAGE_SIZE - 1)) == 0, "Funkcja heap_malloc_aligned() powinien zwrócić adres zaczynający się na początku strony");

                void *ptr2 = heap_malloc_aligned(9061292);
                test_error(ptr2 != NULL, "Funkcja heap_malloc_aligned() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr2), "Funkcja heap_malloc_aligned() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr2));
                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                test_error(((intptr_t)ptr2 & (intptr_t)(PAGE_SIZE - 1)) == 0, "Funkcja heap_malloc_aligned() powinien zwrócić adres zaczynający się na początku strony");

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 92: Sprawdzanie poprawności działania funkcji heap_malloc_aligned - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni większej niż przestrzeń, która może być przydzielona przez system (żądana pamięć wymaga zwiększenia limitu pamięci przydzielonego przez system), w przypadku przydzielenia wcześniej pamięci użytkownikowi
//
void UTEST92(void)
{
    // informacje o teście
    test_start(92, "Sprawdzanie poprawności działania funkcji heap_malloc_aligned - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni większej niż przestrzeń, która może być przydzielona przez system (żądana pamięć wymaga zwiększenia limitu pamięci przydzielonego przez system), w przypadku przydzielenia wcześniej pamięci użytkownikowi", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    rldebug_heap_set_global_limit(0);
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr1 = heap_malloc_aligned(47);
                test_error(((intptr_t)ptr1 & (intptr_t)(PAGE_SIZE - 1)) == 0, "Funkcja heap_malloc_aligned() powinien zwrócić adres zaczynający się na początku strony");
                test_error(ptr1 != NULL, "Funkcja heap_malloc_aligned() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr1), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr1));
                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr2 = heap_malloc_aligned(67837913);
                test_error(ptr2 == NULL, "Funkcja heap_malloc_aligned() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 93: Sprawdzanie poprawności działania funkcji heap_malloc_aligned - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeń mniejszej niż przestrzeń poprzednio zwolniona
//
void UTEST93(void)
{
    // informacje o teście
    test_start(93, "Sprawdzanie poprawności działania funkcji heap_malloc_aligned - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeń mniejszej niż przestrzeń poprzednio zwolniona", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    rldebug_heap_set_global_limit(0);
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr1 = heap_malloc_aligned(92);
                void *ptr2 = heap_malloc_aligned(128);
                void *ptr3 = heap_malloc_aligned(259);

                test_error(ptr1 != NULL, "Funkcja heap_malloc_aligned() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(ptr2 != NULL, "Funkcja heap_malloc_aligned() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(ptr3 != NULL, "Funkcja heap_malloc_aligned() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");

                test_error(pointer_valid == get_pointer_type(ptr1), "Funkcja heap_malloc_aligned() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr1));
                test_error(pointer_valid == get_pointer_type(ptr2), "Funkcja heap_malloc_aligned() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr2));
                test_error(pointer_valid == get_pointer_type(ptr3), "Funkcja heap_malloc_aligned() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr3));

                test_error(((intptr_t)ptr1 & (intptr_t)(PAGE_SIZE - 1)) == 0, "Funkcja heap_malloc_aligned() powinien zwrócić adres zaczynający się na początku strony");
                test_error(((intptr_t)ptr2 & (intptr_t)(PAGE_SIZE - 1)) == 0, "Funkcja heap_malloc_aligned() powinien zwrócić adres zaczynający się na początku strony");
                test_error(((intptr_t)ptr3 & (intptr_t)(PAGE_SIZE - 1)) == 0, "Funkcja heap_malloc_aligned() powinien zwrócić adres zaczynający się na początku strony");


                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                heap_free(ptr2);

                test_error(pointer_unallocated == get_pointer_type(ptr2), "Funkcja heap_malloc_aligned() powinna zwrócić wskaźnik typu pointer_unallocated, a zwróciła %d", get_pointer_type(ptr2));

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr4 = heap_malloc_aligned(128);
                test_error(ptr2 == ptr4, "Funkcja heap_malloc_aligned() powinna zwrócić adres równy adresowi ptr2 (jest możliwość wykorzystania wolnego bloku pamięci do zaalokowania), a zwróciła różny");

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 94: Sprawdzanie poprawności działania funkcji heap_malloc_aligned - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni równej przestrzeni poprzednio zwolnionej
//
void UTEST94(void)
{
    // informacje o teście
    test_start(94, "Sprawdzanie poprawności działania funkcji heap_malloc_aligned - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni równej przestrzeni poprzednio zwolnionej", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    rldebug_heap_set_global_limit(0);
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr1 = heap_malloc_aligned(82);
                void *ptr2 = heap_malloc_aligned(107);
                void *ptr3 = heap_malloc_aligned(237);

                test_error(ptr1 != NULL, "Funkcja heap_malloc_aligned() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(ptr2 != NULL, "Funkcja heap_malloc_aligned() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(ptr3 != NULL, "Funkcja heap_malloc_aligned() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");

                test_error(pointer_valid == get_pointer_type(ptr1), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr1));
                test_error(pointer_valid == get_pointer_type(ptr2), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr2));
                test_error(pointer_valid == get_pointer_type(ptr3), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr3));

                test_error(((intptr_t)ptr1 & (intptr_t)(PAGE_SIZE - 1)) == 0, "Funkcja heap_malloc_aligned() powinien zwrócić adres zaczynający się na początku strony");
                test_error(((intptr_t)ptr2 & (intptr_t)(PAGE_SIZE - 1)) == 0, "Funkcja heap_malloc_aligned() powinien zwrócić adres zaczynający się na początku strony");
                test_error(((intptr_t)ptr3 & (intptr_t)(PAGE_SIZE - 1)) == 0, "Funkcja heap_malloc_aligned() powinien zwrócić adres zaczynający się na początku strony");

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                heap_free(ptr2);

                test_error(pointer_unallocated == get_pointer_type(ptr2), "Funkcja heap_malloc_aligned() powinna zwrócić wskaźnik typu pointer_unallocated, a zwróciła %d", get_pointer_type(ptr2));

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr4 = heap_malloc_aligned(107);
                test_error(ptr2 == ptr4, "Funkcja heap_malloc_aligned() powinna zwrócić adres równy adresowi ptr2 (jest możliwość wykorzystania wolnego bloku pamięci do zaalokowania), a zwróciła różny");

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 95: Sprawdzanie poprawności działania funkcji heap_malloc_aligned - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni równej przestrzeni poprzednio zwolnionej
//
void UTEST95(void)
{
    // informacje o teście
    test_start(95, "Sprawdzanie poprawności działania funkcji heap_malloc_aligned - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni równej przestrzeni poprzednio zwolnionej", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    rldebug_heap_set_global_limit(0);
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr1 = heap_malloc_aligned(98);
                void *ptr2 = heap_malloc_aligned(133);
                void *ptr3 = heap_malloc_aligned(252);

                test_error(ptr1 != NULL, "Funkcja heap_malloc_aligned() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(ptr2 != NULL, "Funkcja heap_malloc_aligned() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(ptr3 != NULL, "Funkcja heap_malloc_aligned() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");

                test_error(pointer_valid == get_pointer_type(ptr1), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr1));
                test_error(pointer_valid == get_pointer_type(ptr2), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr2));
                test_error(pointer_valid == get_pointer_type(ptr3), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr3));

                test_error(((intptr_t)ptr1 & (intptr_t)(PAGE_SIZE - 1)) == 0, "Funkcja heap_malloc_aligned() powinien zwrócić adres zaczynający się na początku strony");
                test_error(((intptr_t)ptr2 & (intptr_t)(PAGE_SIZE - 1)) == 0, "Funkcja heap_malloc_aligned() powinien zwrócić adres zaczynający się na początku strony");
                test_error(((intptr_t)ptr3 & (intptr_t)(PAGE_SIZE - 1)) == 0, "Funkcja heap_malloc_aligned() powinien zwrócić adres zaczynający się na początku strony");

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                heap_free(ptr2);

                test_error(pointer_unallocated == get_pointer_type(ptr2), "Funkcja heap_malloc_aligned() powinna zwrócić wskaźnik typu pointer_unallocated, a zwróciła %d", get_pointer_type(ptr2));

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr4 = heap_malloc_aligned(1250);
                test_error(ptr2 == ptr4, "Funkcja heap_malloc_aligned() powinna zwrócić adres równy adresowi ptr2 (jest możliwość wykorzystania wolnego bloku pamięci do zaalokowania), a zwróciła różny");

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 96: Sprawdzanie poprawności działania funkcji heap_malloc_aligned - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni równej przestrzeni poprzednio zwolnionej
//
void UTEST96(void)
{
    // informacje o teście
    test_start(96, "Sprawdzanie poprawności działania funkcji heap_malloc_aligned - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni równej przestrzeni poprzednio zwolnionej", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    rldebug_heap_set_global_limit(0);
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr1 = heap_malloc_aligned(24);
                void *ptr2 = heap_malloc_aligned(133);
                void *ptr3 = heap_malloc(2158);
                void *ptr4 = heap_malloc(2158);

                test_error(ptr1 != NULL, "Funkcja heap_malloc_aligned() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(ptr2 != NULL, "Funkcja heap_malloc_aligned() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(ptr3 != NULL, "Funkcja heap_malloc_aligned() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(ptr4 != NULL, "Funkcja heap_malloc_aligned() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");

                test_error(pointer_valid == get_pointer_type(ptr1), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr1));
                test_error(pointer_valid == get_pointer_type(ptr2), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr2));
                test_error(pointer_valid == get_pointer_type(ptr3), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr3));
                test_error(pointer_valid == get_pointer_type(ptr4), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr4));

                test_error(((intptr_t)ptr1 & (intptr_t)(PAGE_SIZE - 1)) == 0, "Funkcja heap_malloc_aligned() powinien zwrócić adres zaczynający się na początku strony");
                test_error(((intptr_t)ptr2 & (intptr_t)(PAGE_SIZE - 1)) == 0, "Funkcja heap_malloc_aligned() powinien zwrócić adres zaczynający się na początku strony");
                test_error((intptr_t)ptr3 % sizeof(void *) == 0, "Funkcja heap_malloc() powinien zwrócić adres będący wielokrotnością słowa maszynowego");
                test_error((intptr_t)ptr4 % sizeof(void *) == 0, "Funkcja heap_malloc() powinien zwrócić adres będący wielokrotnością słowa maszynowego");

                test_error((intptr_t)ptr3 < (intptr_t)ptr1, "Funkcja heap_malloc() powinna zwrócić adres znajdujący się przed adresem zwróconym przez funkcję heap_malloc_aligned");
                test_error((intptr_t)ptr4 > (intptr_t)ptr1 && (intptr_t)ptr4 < (intptr_t)ptr2, "Funkcja heap_malloc() powinna zwrócić adres znajdujący się przed adresem zwróconym przez funkcję heap_malloc_aligned");

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                status = custom_sbrk_check_fences_integrity();
                test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                heap_clean();

                uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 97: Sprawdzanie poprawności działania funkcji heap_malloc_aligned - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni równej przestrzeni poprzednio zwolnionej
//
void UTEST97(void)
{
    // informacje o teście
    test_start(97, "Sprawdzanie poprawności działania funkcji heap_malloc_aligned - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni równej przestrzeni poprzednio zwolnionej", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    rldebug_heap_set_global_limit(0);
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr1 = heap_malloc_aligned(4030);
                void *ptr2 = heap_malloc_aligned(164);
                void *ptr3 = heap_malloc_aligned(2057);

                test_error(ptr1 != NULL, "Funkcja heap_malloc_aligned() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(ptr2 != NULL, "Funkcja heap_malloc_aligned() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(ptr3 != NULL, "Funkcja heap_malloc_aligned() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");

                test_error(pointer_valid == get_pointer_type(ptr1), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr1));
                test_error(pointer_valid == get_pointer_type(ptr2), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr2));
                test_error(pointer_valid == get_pointer_type(ptr3), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr3));

                test_error(((intptr_t)ptr1 & (intptr_t)(PAGE_SIZE - 1)) == 0, "Funkcja heap_malloc_aligned() powinien zwrócić adres zaczynający się na początku strony");
                test_error(((intptr_t)ptr2 & (intptr_t)(PAGE_SIZE - 1)) == 0, "Funkcja heap_malloc_aligned() powinien zwrócić adres zaczynający się na początku strony");
                test_error(((intptr_t)ptr3 & (intptr_t)(PAGE_SIZE - 1)) == 0, "Funkcja heap_malloc_aligned() powinien zwrócić adres zaczynający się na początku strony");

                heap_free(ptr2);

                void *ptr4 = heap_malloc_aligned(168);

                test_error(ptr4 != NULL, "Funkcja heap_malloc_aligned() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr4), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr4));
                test_error(((intptr_t)ptr4 & (intptr_t)(PAGE_SIZE - 1)) == 0, "Funkcja heap_malloc_aligned() powinien zwrócić adres zaczynający się na początku strony");

                test_error(ptr4 == ptr2, "Funkcja heap_malloc_aligned() powinna zwrócić adres równy adresowi bloku, który został przed chwilą zwolniony");

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                status = custom_sbrk_check_fences_integrity();
                test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                heap_clean();

                uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 98: Sprawdzanie poprawności działania funkcji heap_malloc_aligned - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni większej niż przestrzeń poprzednio zwolniona
//
void UTEST98(void)
{
    // informacje o teście
    test_start(98, "Sprawdzanie poprawności działania funkcji heap_malloc_aligned - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni większej niż przestrzeń poprzednio zwolniona", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    rldebug_heap_set_global_limit(0);
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                size_t mem_sizes[] = {398, 425, 416, 441, 616, 760, 221, 342, 330, 470, 984, 482, 802, 836, 280, 417, 342, 254, 115, 469, 297, 986, 593, 161, 827, 445, 819, 474, 140, 545, 814, 756, 528, 869, 430, 379, 927};
                void *ptrs[37];

                for (int i = 0; i < 37; ++i)
                {
                    ptrs[i] = heap_malloc_aligned(mem_sizes[i]);
                    test_error(ptrs[i] != NULL, "Funkcja heap_malloc_aligned() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                    test_error(pointer_valid == get_pointer_type(ptrs[i]), "Funkcja heap_malloc_aligned() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptrs[i]));
                    test_error(((intptr_t)ptrs[i] & (intptr_t)(PAGE_SIZE - 1)) == 0, "Funkcja heap_malloc_aligned() powinien zwrócić adres zaczynający się na początku strony");

                    status = heap_validate();
                    test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                }           

                heap_free(ptrs[31]);

                test_error(pointer_unallocated == get_pointer_type(ptrs[31]), "Funkcja heap_malloc_aligned() powinna zwrócić wskaźnik typu pointer_unallocated, a zwróciła %d", get_pointer_type(ptrs[31]));

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr = heap_malloc_aligned(727);
                test_error(ptrs[31] == ptr, "Funkcja heap_malloc_aligned() powinna zwrócić adres równy adresowi ptrs[31] (jest możliwość wykorzystania wolnego bloku pamięci do zaalokowania), a zwróciła różny");

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 99: Sprawdzanie poprawności działania funkcji heap_malloc_aligned - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni równej przestrzeń poprzednio zwolnionej
//
void UTEST99(void)
{
    // informacje o teście
    test_start(99, "Sprawdzanie poprawności działania funkcji heap_malloc_aligned - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni równej przestrzeń poprzednio zwolnionej", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    rldebug_heap_set_global_limit(0);
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                size_t mem_sizes[] = {291, 950, 662, 411, 345, 616, 220, 824, 779, 593, 828, 505, 334, 196, 289, 320, 592, 963, 166, 778, 734, 250, 228, 962};
                void *ptrs[24];

                for (int i = 0; i < 24; ++i)
                {
                    ptrs[i] = heap_malloc_aligned(mem_sizes[i]);
                    test_error(ptrs[i] != NULL, "Funkcja heap_malloc_aligned() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                    test_error(pointer_valid == get_pointer_type(ptrs[i]), "Funkcja heap_malloc_aligned() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptrs[i]));
                    test_error(((intptr_t)ptrs[i] & (intptr_t)(PAGE_SIZE - 1)) == 0, "Funkcja heap_malloc_aligned() powinien zwrócić adres zaczynający się na początku strony");

                    status = heap_validate();
                    test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                }           

                heap_free(ptrs[16]);

                test_error(pointer_unallocated == get_pointer_type(ptrs[16]), "Funkcja heap_malloc_aligned() powinna zwrócić wskaźnik typu pointer_unallocated, a zwróciła %d", get_pointer_type(ptrs[16]));

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr = heap_malloc_aligned(592);
                test_error(ptrs[16] == ptr, "Funkcja heap_malloc_aligned() powinna zwrócić adres równy adresowi ptrs[16] (jest możliwość wykorzystania wolnego bloku pamięci do zaalokowania), a zwróciła różny");

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 100: Sprawdzanie poprawności działania funkcji heap_malloc_aligned - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni większej niż przestrzeń poprzednio zwolnionej
//
void UTEST100(void)
{
    // informacje o teście
    test_start(100, "Sprawdzanie poprawności działania funkcji heap_malloc_aligned - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni większej niż przestrzeń poprzednio zwolnionej", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    rldebug_heap_set_global_limit(0);
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr1 = heap_malloc_aligned(16);
                void *ptr2 = heap_malloc_aligned(153);
                void *ptr3 = heap_malloc_aligned(221);

                test_error(ptr1 != NULL, "Funkcja heap_malloc_aligned() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(ptr2 != NULL, "Funkcja heap_malloc_aligned() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(ptr3 != NULL, "Funkcja heap_malloc_aligned() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");

                test_error(pointer_valid == get_pointer_type(ptr1), "Funkcja heap_malloc_aligned() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr1));
                test_error(pointer_valid == get_pointer_type(ptr2), "Funkcja heap_malloc_aligned() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr2));
                test_error(pointer_valid == get_pointer_type(ptr3), "Funkcja heap_malloc_aligned() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr3));

                test_error(((intptr_t)ptr1 & (intptr_t)(PAGE_SIZE - 1)) == 0, "Funkcja heap_malloc_aligned() powinien zwrócić adres zaczynający się na początku strony");
                test_error(((intptr_t)ptr2 & (intptr_t)(PAGE_SIZE - 1)) == 0, "Funkcja heap_malloc_aligned() powinien zwrócić adres zaczynający się na początku strony");
                test_error(((intptr_t)ptr3 & (intptr_t)(PAGE_SIZE - 1)) == 0, "Funkcja heap_malloc_aligned() powinien zwrócić adres zaczynający się na początku strony");

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                heap_free(ptr2);

                test_error(pointer_unallocated == get_pointer_type(ptr2), "Funkcja heap_malloc_aligned() powinna zwrócić wskaźnik typu pointer_unallocated, a zwróciła %d", get_pointer_type(ptr2));

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr4 = heap_malloc_aligned(4196);

                test_error(ptr2 != ptr4, "Funkcja heap_malloc_aligned() powinna zwrócić adres równy adresowi ptr2 (jest możliwość wykorzystania wolnego bloku pamięci do zaalokowania), a zwróciła różny");
                test_error(ptr4 != NULL, "Funkcja heap_malloc_aligned() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr4), "Funkcja heap_malloc_aligned() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr4));

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 101: Sprawdzanie poprawności działania funkcji heap_malloc_aligned - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni większej niż przestrzeń poprzednio zwolnionej
//
void UTEST101(void)
{
    // informacje o teście
    test_start(101, "Sprawdzanie poprawności działania funkcji heap_malloc_aligned - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni większej niż przestrzeń poprzednio zwolnionej", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    rldebug_heap_set_global_limit(0);
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr1 = heap_malloc_aligned(59);
                void *ptr2 = heap_malloc_aligned(6);
                void *ptr3 = heap_malloc_aligned(272);

                test_error(ptr1 != NULL, "Funkcja heap_malloc_aligned() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(ptr2 != NULL, "Funkcja heap_malloc_aligned() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(ptr3 != NULL, "Funkcja heap_malloc_aligned() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");

                test_error(pointer_valid == get_pointer_type(ptr1), "Funkcja heap_malloc_aligned() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr1));
                test_error(pointer_valid == get_pointer_type(ptr2), "Funkcja heap_malloc_aligned() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr2));
                test_error(pointer_valid == get_pointer_type(ptr3), "Funkcja heap_malloc_aligned() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr3));

                test_error(((intptr_t)ptr1 & (intptr_t)(PAGE_SIZE - 1)) == 0, "Funkcja heap_malloc_aligned() powinien zwrócić adres zaczynający się na początku strony");
                test_error(((intptr_t)ptr2 & (intptr_t)(PAGE_SIZE - 1)) == 0, "Funkcja heap_malloc_aligned() powinien zwrócić adres zaczynający się na początku strony");
                test_error(((intptr_t)ptr3 & (intptr_t)(PAGE_SIZE - 1)) == 0, "Funkcja heap_malloc_aligned() powinien zwrócić adres zaczynający się na początku strony");

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                heap_free(ptr2);

                test_error(pointer_unallocated == get_pointer_type(ptr2), "Funkcja heap_malloc_aligned() powinna zwrócić wskaźnik typu pointer_unallocated, a zwróciła %d", get_pointer_type(ptr2));

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr4 = heap_malloc_aligned(4711);

                test_error(ptr2 != ptr4, "Funkcja heap_malloc_aligned() powinna zwrócić adres równy adresowi ptr2 (jest możliwość wykorzystania wolnego bloku pamięci do zaalokowania), a zwróciła różny");
                test_error(ptr4 != NULL, "Funkcja heap_malloc_aligned() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr4), "Funkcja heap_malloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr4));

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);

            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 102: Sprawdzanie poprawności działania funkcji heap_calloc_aligned - test sprawdza poprawność działania funkcji w przypadku przekazania do niej wartości 0
//
void UTEST102(void)
{
    // informacje o teście
    test_start(102, "Sprawdzanie poprawności działania funkcji heap_calloc_aligned - test sprawdza poprawność działania funkcji w przypadku przekazania do niej wartości 0", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr = heap_calloc_aligned(0, 0);

                test_error(ptr == NULL, "Funkcja heap_calloc_aligned() powinna zwrócić wartość NULL");

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);

            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 103: Sprawdzanie poprawności działania funkcji heap_calloc_aligned - test sprawdza poprawność działania funkcji w przypadku przekazania do niej wartości 0
//
void UTEST103(void)
{
    // informacje o teście
    test_start(103, "Sprawdzanie poprawności działania funkcji heap_calloc_aligned - test sprawdza poprawność działania funkcji w przypadku przekazania do niej wartości 0", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr = heap_calloc_aligned(12, 0);

                test_error(ptr == NULL, "Funkcja heap_calloc_aligned() powinna zwrócić wartość NULL");

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 104: Sprawdzanie poprawności działania funkcji heap_calloc_aligned - test sprawdza poprawność działania funkcji w przypadku przekazania do niej wartości 0
//
void UTEST104(void)
{
    // informacje o teście
    test_start(104, "Sprawdzanie poprawności działania funkcji heap_calloc_aligned - test sprawdza poprawność działania funkcji w przypadku przekazania do niej wartości 0", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr = heap_calloc_aligned(0, 2);

                test_error(ptr == NULL, "Funkcja heap_calloc_aligned() powinna zwrócić wartość NULL");

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);

            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 105: Sprawdzanie poprawności działania funkcji heap_calloc_aligned - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni większej niż przestrzeń obecna na stercie
//
void UTEST105(void)
{
    // informacje o teście
    test_start(105, "Sprawdzanie poprawności działania funkcji heap_calloc_aligned - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni większej niż przestrzeń obecna na stercie", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr = heap_calloc_aligned(67108829, 1);

                test_error(ptr == NULL, "Funkcja heap_calloc_aligned() powinna zwrócić wartość NULL");

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 106: Sprawdzanie poprawności działania funkcji heap_calloc_aligned - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni większej niż przestrzeń obecna na stercie
//
void UTEST106(void)
{
    // informacje o teście
    test_start(106, "Sprawdzanie poprawności działania funkcji heap_calloc_aligned - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni większej niż przestrzeń obecna na stercie", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr = heap_calloc_aligned(16777216, 4);

                test_error(ptr == NULL, "Funkcja heap_calloc_aligned() powinna zwrócić wartość NULL");

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 107: Sprawdzanie poprawności działania funkcji heap_calloc_aligned - test sprawdza poprawność działania funkcji w przypadku pierwszej próby zaalokowania przestrzeni mniejszej niż przestrzeń obecna na stercie
//
void UTEST107(void)
{
    // informacje o teście
    test_start(107, "Sprawdzanie poprawności działania funkcji heap_calloc_aligned - test sprawdza poprawność działania funkcji w przypadku pierwszej próby zaalokowania przestrzeni mniejszej niż przestrzeń obecna na stercie", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    rldebug_heap_set_global_limit(0);
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                char *ptr = heap_calloc_aligned(913, 1);

                test_error(ptr != NULL, "Funkcja heap_calloc_aligned() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr), "Funkcja heap_calloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr));

                for (int i = 0; i < 913; ++i)
                    test_error(ptr[i] == 0, "Funkcja heap_calloc_aligned() powinna zwrócić wskaźnik na obszar pamięci wypełniony wartościami 0");

                test_error(((intptr_t)ptr & (intptr_t)(PAGE_SIZE - 1)) == 0, "Funkcja heap_calloc_aligned() powinien zwrócić adres zaczynający się na początku strony");

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 108: Sprawdzanie poprawności działania funkcji heap_calloc_aligned - test sprawdza poprawność działania funkcji w przypadku pierwszej próby zaalokowania przestrzeni mniejszej niż przestrzeń obecna na stercie
//
void UTEST108(void)
{
    // informacje o teście
    test_start(108, "Sprawdzanie poprawności działania funkcji heap_calloc_aligned - test sprawdza poprawność działania funkcji w przypadku pierwszej próby zaalokowania przestrzeni mniejszej niż przestrzeń obecna na stercie", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    rldebug_heap_set_global_limit(0);
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                char *ptr = heap_calloc_aligned(123, 8);

                test_error(ptr != NULL, "Funkcja heap_calloc_aligned() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr), "Funkcja heap_calloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr));

                for (int i = 0; i < 984; ++i)
                    test_error(ptr[i] == 0, "Funkcja heap_calloc_aligned() powinna zwrócić wskaźnik na obszar pamięci wypełniony wartościami 0");

                test_error(((intptr_t)ptr & (intptr_t)(PAGE_SIZE - 1)) == 0, "Funkcja heap_calloc_aligned() powinien zwrócić adres zaczynający się na początku strony");

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 109: Sprawdzanie poprawności działania funkcji heap_calloc_aligned - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni mniejszej niż przestrzeń obecna na stercie, w przypadku przydzielenia wcześniej pamięci użytkownikowi
//
void UTEST109(void)
{
    // informacje o teście
    test_start(109, "Sprawdzanie poprawności działania funkcji heap_calloc_aligned - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni mniejszej niż przestrzeń obecna na stercie, w przypadku przydzielenia wcześniej pamięci użytkownikowi", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    rldebug_heap_set_global_limit(0);
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                char *ptr1 = heap_calloc_aligned(97, 77);
                test_error(ptr1 != NULL, "Funkcja heap_calloc_aligned() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr1), "Funkcja heap_calloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr1));
                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                for (int i = 0; i < 7469; ++i)
                    test_error(ptr1[i] == 0, "Funkcja heap_calloc_aligned() powinna zwrócić wskaźnik na obszar pamięci wypełniony wartościami 0");

                test_error(((intptr_t)ptr1 & (intptr_t)(PAGE_SIZE - 1)) == 0, "Funkcja heap_calloc_aligned() powinien zwrócić adres zaczynający się na początku strony");

                char *ptr2 = heap_calloc_aligned(159, 77);
                test_error(ptr2 != NULL, "Funkcja heap_calloc_aligned() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr2), "Funkcja heap_calloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr2));
                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                for (int i = 0; i < 12243; ++i)
                    test_error(ptr2[i] == 0, "Funkcja heap_calloc_aligned() powinna zwrócić wskaźnik na obszar pamięci wypełniony wartościami 0");

                test_error(((intptr_t)ptr2 & (intptr_t)(PAGE_SIZE - 1)) == 0, "Funkcja heap_calloc_aligned() powinien zwrócić adres zaczynający się na początku strony");

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 110: Sprawdzanie poprawności działania funkcji heap_calloc_aligned - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni mniejszej niż przestrzeń obecna na stercie (żądana pamięć wymaga zwiększenia limitu pamięci przydzielonego przez system), w przypadku przydzielenia wcześniej pamięci użytkownikowi
//
void UTEST110(void)
{
    // informacje o teście
    test_start(110, "Sprawdzanie poprawności działania funkcji heap_calloc_aligned - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni mniejszej niż przestrzeń obecna na stercie (żądana pamięć wymaga zwiększenia limitu pamięci przydzielonego przez system), w przypadku przydzielenia wcześniej pamięci użytkownikowi", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    rldebug_heap_set_global_limit(0);
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                char *ptr1 = heap_calloc_aligned(27, 4);
                test_error(ptr1 != NULL, "Funkcja heap_calloc_aligned() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr1), "Funkcja heap_calloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr1));
                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                for (int i = 0; i < 108; ++i)
                    test_error(ptr1[i] == 0, "Funkcja heap_calloc_aligned() powinna zwrócić wskaźnik na obszar pamięci wypełniony wartościami 0");

                test_error(((intptr_t)ptr1 & (intptr_t)(PAGE_SIZE - 1)) == 0, "Funkcja heap_calloc_aligned() powinien zwrócić adres zaczynający się na początku strony");

                char *ptr2 = heap_calloc_aligned(11378638, 4);
                test_error(ptr2 != NULL, "Funkcja heap_calloc_aligned() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr2), "Funkcja heap_calloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr2));
                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                for (int i = 0; i < 45514552; ++i)
                    test_error(ptr2[i] == 0, "Funkcja heap_calloc_aligned() powinna zwrócić wskaźnik na obszar pamięci wypełniony wartościami 0");

                test_error(((intptr_t)ptr2 & (intptr_t)(PAGE_SIZE - 1)) == 0, "Funkcja heap_calloc_aligned() powinien zwrócić adres zaczynający się na początku strony");

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 111: Sprawdzanie poprawności działania funkcji heap_calloc_aligned - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni większej niż przestrzeń, która może być przydzielona przez system (żądana pamięć wymaga zwiększenia limitu pamięci przydzielonego przez system), w przypadku przydzielenia wcześniej pamięci użytkownikowi
//
void UTEST111(void)
{
    // informacje o teście
    test_start(111, "Sprawdzanie poprawności działania funkcji heap_calloc_aligned - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni większej niż przestrzeń, która może być przydzielona przez system (żądana pamięć wymaga zwiększenia limitu pamięci przydzielonego przez system), w przypadku przydzielenia wcześniej pamięci użytkownikowi", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    rldebug_heap_set_global_limit(0);
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                char *ptr1 = heap_calloc_aligned(97, 1);
                test_error(ptr1 != NULL, "Funkcja heap_calloc_aligned() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr1), "Funkcja heap_calloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr1));
                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                for (int i = 0; i < 97; ++i)
                    test_error(ptr1[i] == 0, "Funkcja heap_calloc_aligned() powinna zwrócić wskaźnik na obszar pamięci wypełniony wartościami 0");

                test_error(((intptr_t)ptr1 & (intptr_t)(PAGE_SIZE - 1)) == 0, "Funkcja heap_calloc_aligned() powinien zwrócić adres zaczynający się na początku strony");

                char *ptr2 = heap_calloc_aligned(67104773, 1);
                test_error(ptr2 == NULL, "Funkcja heap_calloc_aligned() powinna zwrócić NULL");

                test_error(((intptr_t)ptr2 & (intptr_t)(PAGE_SIZE - 1)) == 0, "Funkcja heap_calloc_aligned() powinien zwrócić adres zaczynający się na początku strony");

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 112: Sprawdzanie poprawności działania funkcji heap_calloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeń mniejszej niż przestrzeń poprzednio zwolniona
//
void UTEST112(void)
{
    // informacje o teście
    test_start(112, "Sprawdzanie poprawności działania funkcji heap_calloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeń mniejszej niż przestrzeń poprzednio zwolniona", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    rldebug_heap_set_global_limit(0);
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                char *ptr1 = heap_calloc_aligned(26, 1);
                char *ptr2 = heap_calloc_aligned(142, 1);
                char *ptr3 = heap_calloc_aligned(279, 1);

                test_error(ptr1 != NULL, "Funkcja heap_calloc_aligned() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(ptr2 != NULL, "Funkcja heap_calloc_aligned() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(ptr3 != NULL, "Funkcja heap_calloc_aligned() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");

                test_error(pointer_valid == get_pointer_type(ptr1), "Funkcja heap_calloc_aligned() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr1));
                test_error(pointer_valid == get_pointer_type(ptr2), "Funkcja heap_calloc_aligned() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr2));
                test_error(pointer_valid == get_pointer_type(ptr3), "Funkcja heap_calloc_aligned() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr3));

                test_error(((intptr_t)ptr1 & (intptr_t)(PAGE_SIZE - 1)) == 0, "Funkcja heap_calloc_aligned() powinien zwrócić adres zaczynający się na początku strony");
                test_error(((intptr_t)ptr2 & (intptr_t)(PAGE_SIZE - 1)) == 0, "Funkcja heap_calloc_aligned() powinien zwrócić adres zaczynający się na początku strony");
                test_error(((intptr_t)ptr3 & (intptr_t)(PAGE_SIZE - 1)) == 0, "Funkcja heap_calloc_aligned() powinien zwrócić adres zaczynający się na początku strony");


                for (int i = 0; i < 26; ++i)
                    test_error(ptr1[i] == 0, "Funkcja heap_calloc_aligned() powinna zwrócić wskaźnik na obszar pamięci wypełniony wartościami 0");

                for (int i = 0; i < 142; ++i)
                    test_error(ptr2[i] == 0, "Funkcja heap_calloc_aligned() powinna zwrócić wskaźnik na obszar pamięci wypełniony wartościami 0");

                for (int i = 0; i < 279; ++i)
                    test_error(ptr3[i] == 0, "Funkcja heap_calloc_aligned() powinna zwrócić wskaźnik na obszar pamięci wypełniony wartościami 0");

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                heap_free(ptr2);

                test_error(pointer_unallocated == get_pointer_type(ptr2), "Funkcja heap_calloc_aligned() powinna zwrócić wskaźnik typu pointer_unallocated, a zwróciła %d", get_pointer_type(ptr2));

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                char *ptr4 = heap_calloc_aligned(130, 1);
                test_error(ptr2 == ptr4, "Funkcja heap_calloc_aligned() powinna zwrócić adres równy adresowi ptr2 (jest możliwość wykorzystania wolnego bloku pamięci do zaalokowania), a zwróciła różny");

                for (int i = 0; i < 130; ++i)
                    test_error(ptr4[i] == 0, "Funkcja heap_calloc_aligned() powinna zwrócić wskaźnik na obszar pamięci wypełniony wartościami 0");

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 113: Sprawdzanie poprawności działania funkcji heap_calloc_aligned - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni równej przestrzeni poprzednio zwolnionej
//
void UTEST113(void)
{
    // informacje o teście
    test_start(113, "Sprawdzanie poprawności działania funkcji heap_calloc_aligned - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni równej przestrzeni poprzednio zwolnionej", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    rldebug_heap_set_global_limit(0);
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                char *ptr1 = heap_calloc_aligned(57, 4);
                char *ptr2 = heap_calloc_aligned(155, 4);
                char *ptr3 = heap_calloc_aligned(212, 4);

                test_error(ptr1 != NULL, "Funkcja heap_calloc_aligned() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(ptr2 != NULL, "Funkcja heap_calloc_aligned() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(ptr3 != NULL, "Funkcja heap_calloc_aligned() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");

                test_error(pointer_valid == get_pointer_type(ptr1), "Funkcja heap_calloc_aligned() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr1));
                test_error(pointer_valid == get_pointer_type(ptr2), "Funkcja heap_calloc_aligned() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr2));
                test_error(pointer_valid == get_pointer_type(ptr3), "Funkcja heap_calloc_aligned() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr3));

                test_error(((intptr_t)ptr1 & (intptr_t)(PAGE_SIZE - 1)) == 0, "Funkcja heap_calloc_aligned() powinien zwrócić adres zaczynający się na początku strony");
                test_error(((intptr_t)ptr2 & (intptr_t)(PAGE_SIZE - 1)) == 0, "Funkcja heap_calloc_aligned() powinien zwrócić adres zaczynający się na początku strony");
                test_error(((intptr_t)ptr3 & (intptr_t)(PAGE_SIZE - 1)) == 0, "Funkcja heap_calloc_aligned() powinien zwrócić adres zaczynający się na początku strony");


                for (int i = 0; i < 228; ++i)
                    test_error(ptr1[i] == 0, "Funkcja heap_calloc_aligned() powinna zwrócić wskaźnik na obszar pamięci wypełniony wartościami 0");

                for (int i = 0; i < 620; ++i)
                    test_error(ptr2[i] == 0, "Funkcja heap_calloc_aligned() powinna zwrócić wskaźnik na obszar pamięci wypełniony wartościami 0");

                for (int i = 0; i < 848; ++i)
                    test_error(ptr3[i] == 0, "Funkcja heap_calloc_aligned() powinna zwrócić wskaźnik na obszar pamięci wypełniony wartościami 0");

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                heap_free(ptr2);

                test_error(pointer_unallocated == get_pointer_type(ptr2), "Funkcja get_pointer_type() powinna zwrócić wskaźnik typu pointer_unallocated, a zwróciła %d", get_pointer_type(ptr2));

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                char *ptr4 = heap_calloc_aligned(155, 4);
                test_error(ptr2 == ptr4, "Funkcja heap_calloc_aligned() powinna zwrócić adres równy adresowi ptr2 (jest możliwość wykorzystania wolnego bloku pamięci do zaalokowania), a zwróciła różny");

                for (int i = 0; i < 620; ++i)
                    test_error(ptr4[i] == 0, "Funkcja heap_calloc_aligned() powinna zwrócić wskaźnik na obszar pamięci wypełniony wartościami 0");

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 114: Sprawdzanie poprawności działania funkcji heap_calloc_aligned - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni równej przestrzeni poprzednio zwolnionej
//
void UTEST114(void)
{
    // informacje o teście
    test_start(114, "Sprawdzanie poprawności działania funkcji heap_calloc_aligned - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni równej przestrzeni poprzednio zwolnionej", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    rldebug_heap_set_global_limit(0);
    
    //
    // -----------
    //
    
                 int status = heap_setup();
                 test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                 char *ptr1 = heap_calloc_aligned(31, 1);
                 char *ptr2 = heap_calloc_aligned(112, 1);
                 char *ptr3 = heap_calloc_aligned(201, 1);

                 test_error(ptr1 != NULL, "Funkcja heap_calloc_aligned() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                 test_error(ptr2 != NULL, "Funkcja heap_calloc_aligned() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                 test_error(ptr3 != NULL, "Funkcja heap_calloc_aligned() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");

                 test_error(pointer_valid == get_pointer_type(ptr1), "Funkcja heap_calloc_aligned() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr1));
                 test_error(pointer_valid == get_pointer_type(ptr2), "Funkcja heap_calloc_aligned() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr2));
                 test_error(pointer_valid == get_pointer_type(ptr3), "Funkcja heap_calloc_aligned() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr3));

                 test_error(((intptr_t)ptr1 & (intptr_t)(PAGE_SIZE - 1)) == 0, "Funkcja heap_calloc_aligned() powinien zwrócić adres zaczynający się na początku strony");
                 test_error(((intptr_t)ptr2 & (intptr_t)(PAGE_SIZE - 1)) == 0, "Funkcja heap_calloc_aligned() powinien zwrócić adres zaczynający się na początku strony");
                 test_error(((intptr_t)ptr3 & (intptr_t)(PAGE_SIZE - 1)) == 0, "Funkcja heap_calloc_aligned() powinien zwrócić adres zaczynający się na początku strony");


                 for (int i = 0; i < 31; ++i)
                     test_error(ptr1[i] == 0, "Funkcja heap_calloc_aligned() powinna zwrócić wskaźnik na obszar pamięci wypełniony wartościami 0");

                 for (int i = 0; i < 112; ++i)
                     test_error(ptr2[i] == 0, "Funkcja heap_calloc_aligned() powinna zwrócić wskaźnik na obszar pamięci wypełniony wartościami 0");

                 for (int i = 0; i < 201; ++i)
                     test_error(ptr3[i] == 0, "Funkcja heap_calloc_aligned() powinna zwrócić wskaźnik na obszar pamięci wypełniony wartościami 0");

                 status = heap_validate();
                 test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                 heap_free(ptr2);

                 test_error(pointer_unallocated == get_pointer_type(ptr2), "Funkcja get_pointer_type() powinna zwrócić wskaźnik typu pointer_unallocated, a zwróciła %d", get_pointer_type(ptr2));

                 status = heap_validate();
                 test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                 char *ptr4 = heap_calloc_aligned(690, 1);
                 test_error(ptr2 == ptr4, "Funkcja heap_calloc_aligned() powinna zwrócić adres równy adresowi ptr2 (jest możliwość wykorzystania wolnego bloku pamięci do zaalokowania), a zwróciła różny");

                 for (int i = 0; i < 690; ++i)
                     test_error(ptr4[i] == 0, "Funkcja heap_calloc_aligned() powinna zwrócić wskaźnik na obszar pamięci wypełniony wartościami 0");

                 status = heap_validate();
                 test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
             
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 115: Sprawdzanie poprawności działania funkcji heap_malloc_aligned - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni równej przestrzeni poprzednio zwolnionej
//
void UTEST115(void)
{
    // informacje o teście
    test_start(115, "Sprawdzanie poprawności działania funkcji heap_malloc_aligned - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni równej przestrzeni poprzednio zwolnionej", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    rldebug_heap_set_global_limit(0);
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr1 = heap_calloc_aligned(33, 1);
                void *ptr2 = heap_calloc_aligned(152, 1);
                void *ptr3 = heap_calloc(2071, 1);
                void *ptr4 = heap_calloc(2071, 1);

                test_error(ptr1 != NULL, "Funkcja heap_calloc_aligned() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(ptr2 != NULL, "Funkcja heap_calloc_aligned() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(ptr3 != NULL, "Funkcja heap_calloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(ptr4 != NULL, "Funkcja heap_calloc() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");

                test_error(pointer_valid == get_pointer_type(ptr1), "Funkcja heap_calloc_aligned() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr1));
                test_error(pointer_valid == get_pointer_type(ptr2), "Funkcja heap_calloc_aligned() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr2));
                test_error(pointer_valid == get_pointer_type(ptr3), "Funkcja heap_calloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr3));
                test_error(pointer_valid == get_pointer_type(ptr4), "Funkcja heap_calloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr4));

                test_error(((intptr_t)ptr1 & (intptr_t)(PAGE_SIZE - 1)) == 0, "Funkcja heap_calloc_aligned() powinien zwrócić adres zaczynający się na początku strony");
                test_error(((intptr_t)ptr2 & (intptr_t)(PAGE_SIZE - 1)) == 0, "Funkcja heap_calloc_aligned() powinien zwrócić adres zaczynający się na początku strony");
                test_error((intptr_t)ptr3 % sizeof(void *) == 0, "Funkcja heap_calloc() powinien zwrócić adres będący wielokrotnością słowa maszynowego");
                test_error((intptr_t)ptr4 % sizeof(void *) == 0, "Funkcja heap_calloc() powinien zwrócić adres będący wielokrotnością słowa maszynowego");

                test_error((intptr_t)ptr3 < (intptr_t)ptr1, "Funkcja heap_calloc() powinna zwrócić adres znajdujący się przed adresem zwróconym przez funkcję heap_malloc_aligned");
                test_error((intptr_t)ptr4 > (intptr_t)ptr1 && (intptr_t)ptr4 < (intptr_t)ptr2, "Funkcja heap_calloc() powinna zwrócić adres znajdujący się przed adresem zwróconym przez funkcję heap_malloc_aligned");

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                status = custom_sbrk_check_fences_integrity();
                test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                heap_clean();

                uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 116: Sprawdzanie poprawności działania funkcji heap_calloc_aligned - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni mniejszej niż przestrzeń poprzednio zwolniona
//
void UTEST116(void)
{
    // informacje o teście
    test_start(116, "Sprawdzanie poprawności działania funkcji heap_calloc_aligned - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni mniejszej niż przestrzeń poprzednio zwolniona", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    rldebug_heap_set_global_limit(0);
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                size_t mem_sizes[] = {212, 460, 477, 256, 208, 435, 851, 499, 448, 625, 364, 667, 275, 402, 713, 726, 253, 261, 964, 449, 218, 403, 980, 748, 134, 224, 542};
                size_t mem_units[] = {5, 3, 1, 2, 3, 3, 5, 5, 4, 3, 3, 1, 2, 3, 5, 5, 1, 2, 1, 5, 1, 4, 1, 3, 5, 1, 4};
                char *ptrs[27];

                for (int i = 0; i < 27; ++i)
                {
                    ptrs[i] = heap_calloc_aligned(mem_sizes[i], mem_units[i]);
                    test_error(ptrs[i] != NULL, "Funkcja heap_calloc_aligned() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                    test_error(pointer_valid == get_pointer_type(ptrs[i]), "Funkcja heap_calloc_aligned() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptrs[i]));

                    test_error(((intptr_t)ptrs[i] & (intptr_t)(PAGE_SIZE - 1)) == 0, "Funkcja heap_calloc_aligned() powinien zwrócić adres zaczynający się na początku strony");

                    for (size_t j = 0; j < mem_sizes[i] * mem_units[i]; ++j)
                        test_error(ptrs[i][j] == 0, "Funkcja heap_calloc_aligned() powinna zwrócić wskaźnik na obszar pamięci wypełniony wartościami 0");

                    status = heap_validate();
                    test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                }

                heap_free(ptrs[8]);

                test_error(pointer_unallocated == get_pointer_type(ptrs[8]), "Funkcja heap_calloc_aligned() powinna zwrócić wskaźnik typu pointer_unallocated, a zwróciła %d", get_pointer_type(ptrs[8]));

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr = heap_calloc_aligned(416, 4);
                test_error(ptrs[8] == ptr, "Funkcja heap_calloc_aligned() powinna zwrócić adres równy adresowi ptrs[8] (jest możliwość wykorzystania wolnego bloku pamięci do zaalokowania), a zwróciła różny");

                for (size_t j = 0; j < 416 * 4; ++j)
                    test_error(ptrs[8][j] == 0, "Funkcja heap_calloc_aligned() powinna zwrócić wskaźnik na obszar pamięci wypełniony wartościami 0");


                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 117: Sprawdzanie poprawności działania funkcji heap_calloc_aligned - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni równej przestrzeni poprzednio zwolnionej
//
void UTEST117(void)
{
    // informacje o teście
    test_start(117, "Sprawdzanie poprawności działania funkcji heap_calloc_aligned - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni równej przestrzeni poprzednio zwolnionej", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    rldebug_heap_set_global_limit(0);
    
    //
    // -----------
    //
    
                 int status = heap_setup();
                 test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                 size_t mem_sizes[] = {610, 801, 688, 248, 218, 966, 465, 625, 494, 271, 280, 785, 269, 552, 167, 637, 557, 558, 717, 868, 191, 987, 430, 706, 342, 955, 771};
                 size_t mem_units[] = {1, 4, 3, 2, 1, 2, 1, 3, 2, 4, 4, 4, 5, 2, 4, 3, 2, 3, 5, 1, 3, 2, 1, 3, 4, 2, 2};
                 char *ptrs[27];

                 for (int i = 0; i < 27; ++i)
                 {
                     ptrs[i] = heap_calloc_aligned(mem_sizes[i], mem_units[i]);
                     test_error(ptrs[i] != NULL, "Funkcja heap_calloc_aligned() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                     test_error(pointer_valid == get_pointer_type(ptrs[i]), "Funkcja heap_calloc() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptrs[i]));

                    test_error(((intptr_t)ptrs[i] & (intptr_t)(PAGE_SIZE - 1)) == 0, "Funkcja heap_calloc_aligned() powinien zwrócić adres zaczynający się na początku strony");

                     for (size_t j = 0; j < mem_sizes[i] * mem_units[i]; ++j)
                         test_error(ptrs[i][j] == 0, "Funkcja heap_calloc_aligned() powinna zwrócić wskaźnik na obszar pamięci wypełniony wartościami 0");

                     status = heap_validate();
                     test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);
                 }

                 heap_free(ptrs[5]);

                 test_error(pointer_unallocated == get_pointer_type(ptrs[5]), "Funkcja heap_calloc() powinna zwrócić wskaźnik typu pointer_unallocated, a zwróciła %d", get_pointer_type(ptrs[5]));

                 status = heap_validate();
                 test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                 void *ptr = heap_calloc_aligned(966, 2);
                 test_error(ptrs[5] == ptr, "Funkcja heap_calloc_aligned() powinna zwrócić adres równy adresowi ptrs[5] (jest możliwość wykorzystania wolnego bloku pamięci do zaalokowania), a zwróciła różny");

                 for (size_t j = 0; j < 966 * 2; ++j)
                     test_error(ptrs[5][j] == 0, "Funkcja heap_calloc_aligned() powinna zwrócić wskaźnik na obszar pamięci wypełniony wartościami 0");


                 status = heap_validate();
                 test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
             
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 118: Sprawdzanie poprawności działania funkcji heap_calloc_aligned - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni większej niż przestrzeń poprzednio zwolnionej
//
void UTEST118(void)
{
    // informacje o teście
    test_start(118, "Sprawdzanie poprawności działania funkcji heap_calloc_aligned - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni większej niż przestrzeń poprzednio zwolnionej", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    rldebug_heap_set_global_limit(0);
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                char *ptr1 = heap_calloc_aligned(62, 1);
                char *ptr2 = heap_calloc_aligned(106, 1);
                char *ptr3 = heap_calloc_aligned(256, 1);

                test_error(ptr1 != NULL, "Funkcja heap_calloc_aligned() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(ptr2 != NULL, "Funkcja heap_calloc_aligned() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(ptr3 != NULL, "Funkcja heap_calloc_aligned() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");

                test_error(pointer_valid == get_pointer_type(ptr1), "Funkcja get_pointer_type() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr1));
                test_error(pointer_valid == get_pointer_type(ptr2), "Funkcja get_pointer_type() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr2));
                test_error(pointer_valid == get_pointer_type(ptr3), "Funkcja get_pointer_type() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr3));

                test_error(((intptr_t)ptr1 & (intptr_t)(PAGE_SIZE - 1)) == 0, "Funkcja heap_calloc_aligned() powinien zwrócić adres zaczynający się na początku strony");
                test_error(((intptr_t)ptr2 & (intptr_t)(PAGE_SIZE - 1)) == 0, "Funkcja heap_calloc_aligned() powinien zwrócić adres zaczynający się na początku strony");
                test_error(((intptr_t)ptr3 & (intptr_t)(PAGE_SIZE - 1)) == 0, "Funkcja heap_calloc_aligned() powinien zwrócić adres zaczynający się na początku strony");

                for (int i = 0; i < 62; ++i)
                    test_error(ptr1[i] == 0, "Funkcja heap_calloc_aligned() powinna zwrócić wskaźnik na obszar pamięci wypełniony wartościami 0");

                for (int i = 0; i < 106; ++i)
                    test_error(ptr2[i] == 0, "Funkcja heap_calloc_aligned() powinna zwrócić wskaźnik na obszar pamięci wypełniony wartościami 0");

                for (int i = 0; i < 256; ++i)
                    test_error(ptr3[i] == 0, "Funkcja heap_calloc_aligned() powinna zwrócić wskaźnik na obszar pamięci wypełniony wartościami 0");

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                heap_free(ptr2);

                test_error(pointer_unallocated == get_pointer_type(ptr2), "Funkcja get_pointer_type() powinna zwrócić wskaźnik typu pointer_unallocated, a zwróciła %d", get_pointer_type(ptr2));

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                char *ptr4 = heap_calloc_aligned(4101, 1);

                test_error(ptr2 != ptr4, "Funkcja heap_calloc_aligned() powinna zwrócić adres różny od adresu ptr2 (jest możliwość wykorzystania wolnego bloku pamięci do zaalokowania), a zwróciła różny");
                test_error(ptr4 != NULL, "Funkcja heap_calloc_aligned() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr4), "Funkcja heap_calloc_aligned() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr4));

                for (int i = 0; i < 4101; ++i)
                    test_error(ptr4[i] == 0, "Funkcja heap_calloc_aligned() powinna zwrócić wskaźnik na obszar pamięci wypełniony wartościami 0");

                test_error(((intptr_t)ptr4 & (intptr_t)(PAGE_SIZE - 1)) == 0, "Funkcja heap_calloc_aligned() powinien zwrócić adres zaczynający się na początku strony");

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 119: Sprawdzanie poprawności działania funkcji heap_calloc_aligned - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni większej niż przestrzeń poprzednio zwolnionej
//
void UTEST119(void)
{
    // informacje o teście
    test_start(119, "Sprawdzanie poprawności działania funkcji heap_calloc_aligned - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni większej niż przestrzeń poprzednio zwolnionej", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    rldebug_heap_set_global_limit(0);
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                char *ptr1 = heap_calloc_aligned(74, 1);
                char *ptr2 = heap_calloc_aligned(9, 1);
                char *ptr3 = heap_calloc_aligned(243, 1);

                test_error(ptr1 != NULL, "Funkcja heap_calloc_aligned() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(ptr2 != NULL, "Funkcja heap_calloc_aligned() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(ptr3 != NULL, "Funkcja heap_calloc_aligned() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");

                test_error(pointer_valid == get_pointer_type(ptr1), "Funkcja get_pointer_type() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr1));
                test_error(pointer_valid == get_pointer_type(ptr2), "Funkcja get_pointer_type() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr2));
                test_error(pointer_valid == get_pointer_type(ptr3), "Funkcja get_pointer_type() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr3));

                test_error(((intptr_t)ptr1 & (intptr_t)(PAGE_SIZE - 1)) == 0, "Funkcja heap_calloc_aligned() powinien zwrócić adres zaczynający się na początku strony");
                test_error(((intptr_t)ptr2 & (intptr_t)(PAGE_SIZE - 1)) == 0, "Funkcja heap_calloc_aligned() powinien zwrócić adres zaczynający się na początku strony");
                test_error(((intptr_t)ptr3 & (intptr_t)(PAGE_SIZE - 1)) == 0, "Funkcja heap_calloc_aligned() powinien zwrócić adres zaczynający się na początku strony");

                for (int i = 0; i < 74; ++i)
                    test_error(ptr1[i] == 0, "Funkcja heap_calloc_aligned() powinna zwrócić wskaźnik na obszar pamięci wypełniony wartościami 0");

                for (int i = 0; i < 9; ++i)
                    test_error(ptr2[i] == 0, "Funkcja heap_calloc_aligned() powinna zwrócić wskaźnik na obszar pamięci wypełniony wartościami 0");

                for (int i = 0; i < 243; ++i)
                    test_error(ptr3[i] == 0, "Funkcja heap_calloc_aligned() powinna zwrócić wskaźnik na obszar pamięci wypełniony wartościami 0");

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                heap_free(ptr2);

                test_error(pointer_unallocated == get_pointer_type(ptr2), "Funkcja get_pointer_type() powinna zwrócić wskaźnik typu pointer_unallocated, a zwróciła %d", get_pointer_type(ptr2));

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                char *ptr4 = heap_calloc_aligned(7027, 1);

                test_error(ptr2 != ptr4, "Funkcja heap_calloc_aligned() powinna zwrócić adres różny od adresu ptr2 (jest możliwość wykorzystania wolnego bloku pamięci do zaalokowania), a zwróciła różny");
                test_error(ptr4 != NULL, "Funkcja heap_calloc_aligned() powinna zwrócić adres przydzielonego bloku pamięci, a zwróciła NULL");
                test_error(pointer_valid == get_pointer_type(ptr4), "Funkcja heap_calloc_aligned() powinna zwrócić wskaźnik typu pointer_valid, a zwróciła %d", get_pointer_type(ptr4));

                for (int i = 0; i < 7027; ++i)
                    test_error(ptr4[i] == 0, "Funkcja heap_calloc_aligned() powinna zwrócić wskaźnik na obszar pamięci wypełniony wartościami 0");

                test_error(((intptr_t)ptr4 & (intptr_t)(PAGE_SIZE - 1)) == 0, "Funkcja heap_calloc_aligned() powinien zwrócić adres zaczynający się na początku strony");

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 120: Sprawdzanie poprawności działania funkcji heap_realloc_aligned - test sprawdza poprawność działania funkcji w przypadku przekazania do niej wartości 0
//
void UTEST120(void)
{
    // informacje o teście
    test_start(120, "Sprawdzanie poprawności działania funkcji heap_realloc_aligned - test sprawdza poprawność działania funkcji w przypadku przekazania do niej wartości 0", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr = heap_realloc_aligned(NULL, 0);

                test_error(ptr == NULL, "Funkcja heap_realloc_aligned() powinna zwrócić wartość NULL");

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 121: Sprawdzanie poprawności działania funkcji heap_realloc_aligned - test sprawdza poprawność działania funkcji w przypadku niezainicjowania sterty
//
void UTEST121(void)
{
    // informacje o teście
    test_start(121, "Sprawdzanie poprawności działania funkcji heap_realloc_aligned - test sprawdza poprawność działania funkcji w przypadku niezainicjowania sterty", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    

                void *ptr = heap_realloc_aligned(NULL, 174);

                test_error(ptr == NULL, "Funkcja heap_realloc_aligned() powinna zwrócić wartość NULL");

                heap_clean();
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 122: Sprawdzanie poprawności działania funkcji heap_realloc_aligned - test sprawdza poprawność działania funkcji w przypadku przekazania do niej poprawnej wartości
//
void UTEST122(void)
{
    // informacje o teście
    test_start(122, "Sprawdzanie poprawności działania funkcji heap_realloc_aligned - test sprawdza poprawność działania funkcji w przypadku przekazania do niej poprawnej wartości", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr = heap_realloc_aligned(NULL, 199);

                test_error(ptr != NULL, "Funkcja heap_realloc_aligned() powinna zwrócić adres pamięci przydzielonej użytkownikowi");
                test_error(get_pointer_type(ptr) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr));

                test_error(((intptr_t)ptr & (intptr_t)(PAGE_SIZE - 1)) == 0, "Funkcja heap_realloc_aligned() powinien zwrócić adres zaczynający się na początku strony");

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 123: Sprawdzanie poprawności działania funkcji heap_realloc_aligned - test sprawdza poprawność działania funkcji w przypadku przekazania do niej wartości 0
//
void UTEST123(void)
{
    // informacje o teście
    test_start(123, "Sprawdzanie poprawności działania funkcji heap_realloc_aligned - test sprawdza poprawność działania funkcji w przypadku przekazania do niej wartości 0", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                void *ptr = heap_realloc_aligned(NULL, 196);

                test_error(ptr != NULL, "Funkcja heap_realloc_aligned() powinna zwrócić adres pamięci przydzielonej użytkownikowi");
                test_error(get_pointer_type(ptr) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr));

                test_error(((intptr_t)ptr & (intptr_t)(PAGE_SIZE - 1)) == 0, "Funkcja heap_realloc_aligned() powinien zwrócić adres zaczynający się na początku strony");

                void *ptr1 = heap_realloc_aligned(ptr, 0);

                test_error(ptr1 == NULL, "Funkcja heap_realloc_aligned() powinna zwrócić wartość NULL");
                test_error(get_pointer_type(ptr) == pointer_unallocated, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_unallocated, a zwróciła %d", get_pointer_type(ptr));

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 124: Sprawdzanie poprawności działania funkcji heap_realloc_aligned - test sprawdza poprawność działania funkcji w przypadku przekazania do niej niewłaściwego wskaźnika
//
void UTEST124(void)
{
    // informacje o teście
    test_start(124, "Sprawdzanie poprawności działania funkcji heap_realloc_aligned - test sprawdza poprawność działania funkcji w przypadku przekazania do niej niewłaściwego wskaźnika", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                char *ptr = heap_realloc_aligned(NULL, 112);

                test_error(ptr != NULL, "Funkcja heap_realloc_aligned() powinna zwrócić adres pamięci przydzielonej użytkownikowi");
                test_error(get_pointer_type(ptr) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr));

                test_error(((intptr_t)ptr & (intptr_t)(PAGE_SIZE - 1)) == 0, "Funkcja heap_realloc_aligned() powinien zwrócić adres zaczynający się na początku strony");

                void *ptr1 = heap_realloc_aligned(ptr + 16, 112);

                test_error(ptr1 == NULL, "Funkcja heap_realloc_aligned() powinna zwrócić wartość NULL");
                test_error(get_pointer_type(ptr) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr));

                ptr1 = heap_realloc_aligned(ptr - 96, 112);

                test_error(ptr1 == NULL, "Funkcja heap_realloc_aligned() powinna zwrócić wartość NULL");
                test_error(get_pointer_type(ptr) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr));

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 125: Sprawdzanie poprawności działania funkcji heap_realloc_aligned - test sprawdza poprawność działania funkcji w przypadku żądania zmniejszenia rozmiaru pamięci
//
void UTEST125(void)
{
    // informacje o teście
    test_start(125, "Sprawdzanie poprawności działania funkcji heap_realloc_aligned - test sprawdza poprawność działania funkcji w przypadku żądania zmniejszenia rozmiaru pamięci", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                unsigned char values[172];

                for (int i = 0; i < 172; ++i)
                    values[i] = rand() % 255;

                char *ptr = heap_realloc_aligned(NULL, 172);
                char *ptr2 = heap_realloc_aligned(NULL, 172);

                test_error(ptr != NULL, "Funkcja heap_realloc_aligned() powinna zwrócić adres pamięci przydzielonej użytkownikowi");
                test_error(get_pointer_type(ptr) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr));

                test_error(ptr2 != NULL, "Funkcja heap_realloc_aligned() powinna zwrócić adres pamięci przydzielonej użytkownikowi");
                test_error(get_pointer_type(ptr2) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr2));

                test_error(((intptr_t)ptr & (intptr_t)(PAGE_SIZE - 1)) == 0, "Funkcja heap_realloc_aligned() powinien zwrócić adres zaczynający się na początku strony");
                test_error(((intptr_t)ptr2 & (intptr_t)(PAGE_SIZE - 1)) == 0, "Funkcja heap_realloc_aligned() powinien zwrócić adres zaczynający się na początku strony");

                memcpy(ptr, values, 172);

                void *ptr1 = heap_realloc_aligned(ptr, 77);

                test_error(ptr1 == ptr, "Funkcja heap_realloc_aligned() powinna zwrócić ten sam adres, który został do niej przekazany");
                test_error(memcmp(ptr, values, 77) == 0, "Funkcja heap_realloc_aligned() nie powinna modyfikować obszaru pamięci użytkownika");

                test_error(get_pointer_type(ptr + 77) == pointer_inside_fences, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_inside_fences, a zwróciła %d", get_pointer_type(ptr + 77));

                 int i = 77;

                char temp = ptr[i];
                ptr[i] += 17;

                status = heap_validate();
                test_error(status == 1, "Funkcja heap_validate() powinna zwrócić wartość 1, a zwróciła na %d", status);

                ptr[i] = temp;

                status = heap_validate();
                test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                while (get_pointer_type(ptr + i) == pointer_inside_fences) ++i;

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 126: Sprawdzanie poprawności działania funkcji heap_realloc_aligned - test sprawdza poprawność działania funkcji w przypadku żądania tego samego rozmiaru pamięci, który zajmował poprzednio
//
void UTEST126(void)
{
    // informacje o teście
    test_start(126, "Sprawdzanie poprawności działania funkcji heap_realloc_aligned - test sprawdza poprawność działania funkcji w przypadku żądania tego samego rozmiaru pamięci, który zajmował poprzednio", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                 int status = heap_setup();
                 test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                 unsigned char values[185];

                 for (int i = 0; i < 185; ++i)
                     values[i] = rand() % 255;

                 char *ptr = heap_realloc_aligned(NULL, 185);
                 char *ptr2 = heap_realloc_aligned(NULL, 185);

                 test_error(ptr != NULL, "Funkcja heap_realloc() powinna zwrócić adres pamięci przydzielonej użytkownikowi");
                 test_error(get_pointer_type(ptr) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr));

                 test_error(ptr2 != NULL, "Funkcja heap_realloc() powinna zwrócić adres pamięci przydzielonej użytkownikowi");
                 test_error(get_pointer_type(ptr2) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr2));

                 test_error(((intptr_t)ptr & (intptr_t)(PAGE_SIZE - 1)) == 0, "Funkcja heap_realloc_aligned() powinien zwrócić adres zaczynający się na początku strony");
                 test_error(((intptr_t)ptr2 & (intptr_t)(PAGE_SIZE - 1)) == 0, "Funkcja heap_realloc_aligned() powinien zwrócić adres zaczynający się na początku strony");

                 memcpy(ptr, values, 185);

                 void *ptr1 = heap_realloc(ptr, 185);

                 test_error(ptr1 == ptr, "Funkcja heap_realloc() powinna zwrócić ten sam adres, który został do niej przekazany");
                 test_error(memcmp(ptr, values, 185) == 0, "Funkcja heap_realloc() nie powinna modyfikować obszaru pamięci użytkownika");

                 test_error(get_pointer_type(ptr + 185) == pointer_inside_fences, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_inside_fences, a zwróciła %d", get_pointer_type(ptr + 185));

                  int i = 185;

                 char temp = ptr[i];
                 ptr[i] += 17;

                 status = heap_validate();
                 test_error(status == 1, "Funkcja heap_validate() powinna zwrócić wartość 1, a zwróciła na %d", status);

                 ptr[i] = temp;

                 status = heap_validate();
                 test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
             
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 127: Sprawdzanie poprawności działania funkcji heap_realloc_aligned - test sprawdza poprawność działania funkcji w przypadku żądania większego rozmiaru pamięci, który zajmował poprzednio
//
void UTEST127(void)
{
    // informacje o teście
    test_start(127, "Sprawdzanie poprawności działania funkcji heap_realloc_aligned - test sprawdza poprawność działania funkcji w przypadku żądania większego rozmiaru pamięci, który zajmował poprzednio", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                 int status = heap_setup();
                 test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                 unsigned char values[118];

                 for (int i = 0; i < 118; ++i)
                     values[i] = rand() % 255;

                 char *ptr = heap_realloc_aligned(NULL, 118);

                 test_error(ptr != NULL, "Funkcja heap_realloc_aligned() powinna zwrócić adres pamięci przydzielonej użytkownikowi");
                 test_error(get_pointer_type(ptr) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr));

                 test_error(((intptr_t)ptr & (intptr_t)(PAGE_SIZE - 1)) == 0, "Funkcja heap_realloc_aligned() powinien zwrócić adres zaczynający się na początku strony");

                 memcpy(ptr, values, 118);

                 void *ptr1 = heap_realloc_aligned(ptr, 911);

                 test_error(ptr1 == ptr, "Funkcja heap_realloc_aligned() powinna zwrócić ten sam adres, który został do niej przekazany");
                 test_error(memcmp(ptr, values, 118) == 0, "Funkcja heap_realloc_aligned() nie powinna modyfikować obszaru pamięci użytkownika");

                 test_error(get_pointer_type(ptr + 911) == pointer_inside_fences, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_inside_fences, a zwróciła %d", get_pointer_type(ptr + 185));

                  int i = 911;

                 char temp = ptr[i];
                 ptr[i] += 17;

                 status = heap_validate();
                 test_error(status == 1, "Funkcja heap_validate() powinna zwrócić wartość 1, a zwróciła na %d", status);

                 ptr[i] = temp;

                 status = heap_validate();
                 test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
             
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 128: Sprawdzanie poprawności działania funkcji heap_realloc_aligned - test sprawdza poprawność działania funkcji w przypadku żądania większego rozmiaru pamięci, który zajmował poprzednio
//
void UTEST128(void)
{
    // informacje o teście
    test_start(128, "Sprawdzanie poprawności działania funkcji heap_realloc_aligned - test sprawdza poprawność działania funkcji w przypadku żądania większego rozmiaru pamięci, który zajmował poprzednio", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                 int status = heap_setup();
                 test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                 unsigned char values[119];

                 for (int i = 0; i < 119; ++i)
                     values[i] = rand() % 255;

                 char *ptr = heap_realloc_aligned(NULL, 119);

                 test_error(ptr != NULL, "Funkcja heap_realloc_aligned() powinna zwrócić adres pamięci przydzielonej użytkownikowi");
                 test_error(get_pointer_type(ptr) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr));

                 test_error(((intptr_t)ptr & (intptr_t)(PAGE_SIZE - 1)) == 0, "Funkcja heap_realloc_aligned() powinien zwrócić adres zaczynający się na początku strony");

                 memcpy(ptr, values, 119);

                 void *ptr1 = heap_realloc_aligned(ptr, 11096769);

                 test_error(ptr1 == ptr, "Funkcja heap_realloc_aligned() powinna zwrócić ten sam adres, który został do niej przekazany");
                 test_error(memcmp(ptr, values, 119) == 0, "Funkcja heap_realloc_aligned() nie powinna modyfikować obszaru pamięci użytkownika");

                 test_error(get_pointer_type(ptr + 11096769) == pointer_inside_fences, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_inside_fences, a zwróciła %d", get_pointer_type(ptr + 185));

                  int i = 11096769;

                 char temp = ptr[i];
                 ptr[i] += 17;

                 status = heap_validate();
                 test_error(status == 1, "Funkcja heap_validate() powinna zwrócić wartość 1, a zwróciła na %d", status);

                 ptr[i] = temp;

                 status = heap_validate();
                 test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
             
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 129: Sprawdzanie poprawności działania funkcji heap_realloc_aligned - test sprawdza poprawność działania funkcji w przypadku żądania większego rozmiaru pamięci, który zajmował poprzednio
//
void UTEST129(void)
{
    // informacje o teście
    test_start(129, "Sprawdzanie poprawności działania funkcji heap_realloc_aligned - test sprawdza poprawność działania funkcji w przypadku żądania większego rozmiaru pamięci, który zajmował poprzednio", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                  int status = heap_setup();
                  test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                  unsigned char values[172];

                  for (int i = 0; i < 172; ++i)
                      values[i] = rand() % 255;

                  char *ptr1 = heap_realloc_aligned(NULL, 172);
                  char *ptr2 = heap_realloc_aligned(NULL, 1392);
                  char *ptr3 = heap_realloc_aligned(NULL, 189);
                  char *ptr4 = heap_realloc_aligned(NULL, 158);

                  test_error(get_pointer_type(ptr1) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr1));
                  test_error(get_pointer_type(ptr2) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr2));
                  test_error(get_pointer_type(ptr3) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr3));
                  test_error(get_pointer_type(ptr4) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr4));

                  test_error(((intptr_t)ptr1 & (intptr_t)(PAGE_SIZE - 1)) == 0, "Funkcja heap_realloc_aligned() powinien zwrócić adres zaczynający się na początku strony");
                  test_error(((intptr_t)ptr2 & (intptr_t)(PAGE_SIZE - 1)) == 0, "Funkcja heap_realloc_aligned() powinien zwrócić adres zaczynający się na początku strony");
                  test_error(((intptr_t)ptr3 & (intptr_t)(PAGE_SIZE - 1)) == 0, "Funkcja heap_realloc_aligned() powinien zwrócić adres zaczynający się na początku strony");
                  test_error(((intptr_t)ptr4 & (intptr_t)(PAGE_SIZE - 1)) == 0, "Funkcja heap_realloc_aligned() powinien zwrócić adres zaczynający się na początku strony");

                  heap_free(ptr2);

                  ptr2 = heap_realloc_aligned(NULL, 172);

                  heap_free(ptr3);

                  memcpy(ptr2, values, 172);

                  char *ptr5 = heap_realloc_aligned(ptr2, 5705);

                  test_error(ptr5 == ptr2, "Funkcja heap_realloc_aligned() powinna zwrócić ten sam adres, który został do niej przekazany");
                  test_error(memcmp(ptr2, values, 172) == 0, "Funkcja heap_realloc_aligned() nie powinna modyfikować obszaru pamięci użytkownika");

                  test_error(get_pointer_type(ptr2 + 5705) == pointer_inside_fences, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_inside_fences, a zwróciła %d", get_pointer_type(ptr2 + 5705));

                  status = heap_validate();
                  test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
              
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 130: Sprawdzanie poprawności działania funkcji heap_realloc_aligned - test sprawdza poprawność działania funkcji w przypadku żądania większego rozmiaru pamięci, który zajmował poprzednio
//
void UTEST130(void)
{
    // informacje o teście
    test_start(130, "Sprawdzanie poprawności działania funkcji heap_realloc_aligned - test sprawdza poprawność działania funkcji w przypadku żądania większego rozmiaru pamięci, który zajmował poprzednio", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                  int status = heap_setup();
                  test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                  unsigned char values[193];

                  for (int i = 0; i < 193; ++i)
                      values[i] = rand() % 255;

                  char *ptr1 = heap_realloc_aligned(NULL, 193);
                  char *ptr2 = heap_realloc_aligned(NULL, 1568);
                  char *ptr3 = heap_realloc_aligned(NULL, 150);

                  test_error(get_pointer_type(ptr1) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr1));
                  test_error(get_pointer_type(ptr2) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr2));
                  test_error(get_pointer_type(ptr3) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr3));

                  test_error(((intptr_t)ptr1 & (intptr_t)(PAGE_SIZE - 1)) == 0, "Funkcja heap_realloc_aligned() powinien zwrócić adres zaczynający się na początku strony");
                  test_error(((intptr_t)ptr2 & (intptr_t)(PAGE_SIZE - 1)) == 0, "Funkcja heap_realloc_aligned() powinien zwrócić adres zaczynający się na początku strony");
                  test_error(((intptr_t)ptr3 & (intptr_t)(PAGE_SIZE - 1)) == 0, "Funkcja heap_realloc_aligned() powinien zwrócić adres zaczynający się na początku strony");

                  memcpy(ptr2, values, 1568);

                  char *ptr5 = heap_realloc_aligned(ptr2, 1680);

                  test_error(ptr5 == ptr2, "Funkcja heap_realloc_aligned() powinna zwrócić ten sam adres, który został do niej przekazany");
                  test_error(memcmp(ptr2, values, 193) == 0, "Funkcja heap_realloc_aligned() nie powinna modyfikować obszaru pamięci użytkownika");

                  test_error(get_pointer_type(ptr2 + 1680) == pointer_inside_fences, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_inside_fences, a zwróciła %d", get_pointer_type(ptr2 + 1680));

                  status = heap_validate();
                  test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
              
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 131: Sprawdzanie poprawności działania funkcji heap_realloc_aligned - test sprawdza poprawność działania funkcji w przypadku żądania większego rozmiaru pamięci, który zajmował poprzednio
//
void UTEST131(void)
{
    // informacje o teście
    test_start(131, "Sprawdzanie poprawności działania funkcji heap_realloc_aligned - test sprawdza poprawność działania funkcji w przypadku żądania większego rozmiaru pamięci, który zajmował poprzednio", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                  int status = heap_setup();
                  test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                  unsigned char values[158];

                  for (int i = 0; i < 158; ++i)
                      values[i] = rand() % 255;

                  char *ptr1 = heap_realloc_aligned(NULL, 158);
                  char *ptr2 = heap_realloc_aligned(NULL, 1374);
                  char *ptr3 = heap_realloc_aligned(NULL, 123);
                  char *ptr4 = heap_realloc_aligned(NULL, 183);

                  test_error(get_pointer_type(ptr1) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr1));
                  test_error(get_pointer_type(ptr2) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr2));
                  test_error(get_pointer_type(ptr3) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr3));
                  test_error(get_pointer_type(ptr4) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr4));

                  test_error(((intptr_t)ptr1 & (intptr_t)(PAGE_SIZE - 1)) == 0, "Funkcja heap_realloc_aligned() powinien zwrócić adres zaczynający się na początku strony");
                  test_error(((intptr_t)ptr2 & (intptr_t)(PAGE_SIZE - 1)) == 0, "Funkcja heap_realloc_aligned() powinien zwrócić adres zaczynający się na początku strony");
                  test_error(((intptr_t)ptr3 & (intptr_t)(PAGE_SIZE - 1)) == 0, "Funkcja heap_realloc_aligned() powinien zwrócić adres zaczynający się na początku strony");
                  test_error(((intptr_t)ptr4 & (intptr_t)(PAGE_SIZE - 1)) == 0, "Funkcja heap_realloc_aligned() powinien zwrócić adres zaczynający się na początku strony");

                  heap_free(ptr2);

                  ptr2 = heap_realloc_aligned(NULL, 158);

                  test_error(get_pointer_type(ptr2) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr2));

                  heap_free(ptr3);

                  memcpy(ptr2, values, 158);

                  char *ptr5 = heap_realloc_aligned(ptr2, 38026);

                  test_error(ptr5 != ptr2, "Funkcja heap_realloc_aligned() powinna zwrócić ten sam adres, który został do niej przekazany");
                  test_error(memcmp(ptr5, values, 158) == 0, "Funkcja heap_realloc_aligned() nie powinna modyfikować obszaru pamięci użytkownika");

                  test_error(get_pointer_type(ptr5 + 38026) == pointer_inside_fences, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_inside_fences, a zwróciła %d", get_pointer_type(ptr5 + 38026));
                  test_error(get_pointer_type(ptr2) == pointer_unallocated, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_unallocated, a zwróciła %d", get_pointer_type(ptr2));

                  test_error(((intptr_t)ptr5 & (intptr_t)(PAGE_SIZE - 1)) == 0, "Funkcja heap_realloc_aligned() powinien zwrócić adres zaczynający się na początku strony");

                  status = heap_validate();
                  test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
              
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 132: Sprawdzanie poprawności działania funkcji heap_realloc_aligned - test sprawdza poprawność działania funkcji w przypadku żądania większego rozmiaru pamięci, który zajmował poprzednio
//
void UTEST132(void)
{
    // informacje o teście
    test_start(132, "Sprawdzanie poprawności działania funkcji heap_realloc_aligned - test sprawdza poprawność działania funkcji w przypadku żądania większego rozmiaru pamięci, który zajmował poprzednio", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                  int status = heap_setup();
                  test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                  unsigned char values[164];

                  for (int i = 0; i < 164; ++i)
                      values[i] = rand() % 255;

                  char *ptr1 = heap_realloc_aligned(NULL, 164);
                  char *ptr2 = heap_realloc_aligned(NULL, 1962);
                  char *ptr3 = heap_realloc_aligned(NULL, 131);
                  char *ptr4 = heap_realloc_aligned(NULL, 126);

                  test_error(get_pointer_type(ptr1) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr1));
                  test_error(get_pointer_type(ptr2) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr2));
                  test_error(get_pointer_type(ptr3) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr3));
                  test_error(get_pointer_type(ptr4) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr4));

                  test_error(((intptr_t)ptr1 & (intptr_t)(PAGE_SIZE - 1)) == 0, "Funkcja heap_realloc_aligned() powinien zwrócić adres zaczynający się na początku strony");
                  test_error(((intptr_t)ptr2 & (intptr_t)(PAGE_SIZE - 1)) == 0, "Funkcja heap_realloc_aligned() powinien zwrócić adres zaczynający się na początku strony");
                  test_error(((intptr_t)ptr3 & (intptr_t)(PAGE_SIZE - 1)) == 0, "Funkcja heap_realloc_aligned() powinien zwrócić adres zaczynający się na początku strony");
                  test_error(((intptr_t)ptr4 & (intptr_t)(PAGE_SIZE - 1)) == 0, "Funkcja heap_realloc_aligned() powinien zwrócić adres zaczynający się na początku strony");

                  heap_free(ptr2);

                  ptr2 = heap_realloc_aligned(NULL, 164);

                  heap_free(ptr3);

                  memcpy(ptr2, values, 164);

                  char *ptr5 = heap_realloc_aligned(ptr2, 67468124);

                  test_error(memcmp(ptr2, values, 164) == 0, "Funkcja heap_realloc_aligned() nie powinna modyfikować obszaru pamięci użytkownika");

                  test_error(get_pointer_type(ptr2) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr2));
                  test_error(get_pointer_type(ptr5) == pointer_null, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_null, a zwróciła %d", get_pointer_type(ptr5));

                  status = heap_validate();
                  test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
              
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 133: Sprawdzanie poprawności działania funkcji heap_realloc_aligned - test sprawdza poprawność działania funkcji w przypadku żądania większego rozmiaru pamięci, który zajmował poprzednio
//
void UTEST133(void)
{
    // informacje o teście
    test_start(133, "Sprawdzanie poprawności działania funkcji heap_realloc_aligned - test sprawdza poprawność działania funkcji w przypadku żądania większego rozmiaru pamięci, który zajmował poprzednio", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                  int status = heap_setup();
                  test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                  unsigned char values[104];

                  for (int i = 0; i < 104; ++i)
                      values[i] = rand() % 255;

                  char *ptr1 = heap_realloc_aligned(NULL, 169);
                  char *ptr2 = heap_realloc_aligned(NULL, 1251);
                  char *ptr3 = heap_realloc_aligned(NULL, 191);
                  char *ptr4 = heap_realloc_aligned(NULL, 104);

                  test_error(get_pointer_type(ptr1) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr1));
                  test_error(get_pointer_type(ptr2) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr2));
                  test_error(get_pointer_type(ptr3) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr3));
                  test_error(get_pointer_type(ptr4) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr4));

                  test_error(((intptr_t)ptr1 & (intptr_t)(PAGE_SIZE - 1)) == 0, "Funkcja heap_realloc_aligned() powinien zwrócić adres zaczynający się na początku strony");
                  test_error(((intptr_t)ptr2 & (intptr_t)(PAGE_SIZE - 1)) == 0, "Funkcja heap_realloc_aligned() powinien zwrócić adres zaczynający się na początku strony");
                  test_error(((intptr_t)ptr3 & (intptr_t)(PAGE_SIZE - 1)) == 0, "Funkcja heap_realloc_aligned() powinien zwrócić adres zaczynający się na początku strony");
                  test_error(((intptr_t)ptr4 & (intptr_t)(PAGE_SIZE - 1)) == 0, "Funkcja heap_realloc_aligned() powinien zwrócić adres zaczynający się na początku strony");


                  memcpy(ptr4, values, 104);

                  char *ptr5 = heap_realloc_aligned(ptr4, 68032945);

                  test_error(memcmp(ptr4, values, 104) == 0, "Funkcja heap_realloc_aligned() nie powinna modyfikować obszaru pamięci użytkownika");

                  test_error(get_pointer_type(ptr4) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr2));
                  test_error(get_pointer_type(ptr5) == pointer_null, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_null, a zwróciła %d", get_pointer_type(ptr5));

                  test_error(get_pointer_type(ptr4 + 104) == pointer_inside_fences, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_inside_fences, a zwróciła %d", get_pointer_type(ptr4 + 104));

                  status = heap_validate();
                  test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
              
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 134: Sprawdzanie poprawności działania funkcji heap_realloc_aligned - test sprawdza poprawność kolejności przydzielania bloków pamięci
//
void UTEST134(void)
{
    // informacje o teście
    test_start(134, "Sprawdzanie poprawności działania funkcji heap_realloc_aligned - test sprawdza poprawność kolejności przydzielania bloków pamięci", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                  int status = heap_setup();
                  test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                  char *ptr1 = heap_realloc_aligned(NULL, 165);
                  char *ptr2 = heap_realloc_aligned(NULL, 174);
                  char *ptr3 = heap_malloc(2438);
                  char *ptr4 = heap_malloc(2438);
                  char *ptr5 = heap_malloc(174);

                  test_error(get_pointer_type(ptr1) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr1));
                  test_error(get_pointer_type(ptr2) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr2));
                  test_error(get_pointer_type(ptr3) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr3));
                  test_error(get_pointer_type(ptr4) == pointer_valid, "Funkcja get_pointer_type() powinna zwrócić wartość pointer_valid, a zwróciła %d", get_pointer_type(ptr4));

                  test_error(((intptr_t)ptr1 & (intptr_t)(PAGE_SIZE - 1)) == 0, "Funkcja heap_realloc_aligned() powinien zwrócić adres zaczynający się na początku strony");
                  test_error(((intptr_t)ptr2 & (intptr_t)(PAGE_SIZE - 1)) == 0, "Funkcja heap_realloc_aligned() powinien zwrócić adres zaczynający się na początku strony");
                  test_error((intptr_t)ptr3 % sizeof(void *) == 0, "Funkcja heap_malloc() powinien zwrócić adres będący wielokrotnością długości słowa maszynowego");
                  test_error((intptr_t)ptr4 % sizeof(void *) == 0, "Funkcja heap_malloc() powinien zwrócić adres będący wielokrotnością długości słowa maszynowego");
                  test_error((intptr_t)ptr5 % sizeof(void *) == 0, "Funkcja heap_malloc() powinien zwrócić adres będący wielokrotnością długości słowa maszynowego");

                  test_error((intptr_t)ptr3 < (intptr_t)ptr1, "Funkcja heap_malloc() powinien zwrócić adres o wartości mniejszej niż adres zwrócony przez pierwsze wywołanie funkcji heap_realloc_aligned");
                  test_error((intptr_t)ptr4 < (intptr_t)ptr2 && (intptr_t)ptr4 > (intptr_t)ptr1, "Funkcja heap_malloc() powinien zwrócić adres o wartości większej niż adres zwrócony przez pierwsze wywołanie funkcji heap_realloc_aligned oraz mniejszy od adresu zwróconego przez drugie wyowłanie tej funkcji");
                  test_error((intptr_t)ptr5 < (intptr_t)ptr1 && (intptr_t)ptr5 > (intptr_t)ptr3, "Funkcja heap_malloc() powinien zwrócić adres o wartości większej niż adres zwrócony przez pierwsze wywołanie funkcji heap_realloc_aligned oraz mniejszy od adresu zwróconego przez drugie wyowłanie tej funkcji");


                  status = heap_validate();
                  test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);
              
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 135: Sprawdzanie poprawności działania funkcji wszystkich funkcji alokujących pamięć
//
void UTEST135(void)
{
    // informacje o teście
    test_start(135, "Sprawdzanie poprawności działania funkcji wszystkich funkcji alokujących pamięć", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                   srand (time(NULL));

                   int status = heap_setup();
                   test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                   char *ptr[5955];
                   int ptr_state[5955] = {0};

                   int is_allocated = 0;

                   for (int i = 0; i < 5955; ++i)
                   {
                     int rand_value = rand() % 100;
                     if (rand_value < 10)
                     {
                       for (int j = 0; j < 5955; ++j)
                         if (ptr_state[j] == 0)
                         {
                           ptr_state[j] = 1;
                           ptr[j] = heap_realloc_aligned(NULL, rand() % 1000 + 500);  
                           test_error(((intptr_t)ptr[j] & (intptr_t)(PAGE_SIZE - 1)) == 0, "Funkcja heap_realloc_aligned() powinien zwrócić adres zaczynający się na początku strony");
                           is_allocated++;
                           break;
                         }
                     }
                     else if (rand_value < 20)
                     {
                       for (int j = 0; j < 5955; ++j)
                         if (ptr_state[j] == 0)
                         {
                           ptr_state[j] = 1;
                           ptr[j] = heap_calloc_aligned(rand() % 1000 + 500, rand() % 4 + 1);  
                           test_error(((intptr_t)ptr[j] & (intptr_t)(PAGE_SIZE - 1)) == 0, "Funkcja heap_calloc_aligned() powinien zwrócić adres zaczynający się na początku strony");
                           is_allocated++;
                           break;
                         }
                     }
                     else if (rand_value < 30)
                     {
                       for (int j = 0; j < 5955; ++j)
                         if (ptr_state[j] == 0)
                         {
                           ptr_state[j] = 1;
                           ptr[j] = heap_malloc_aligned(rand() % 1000 + 500);  
                           test_error(((intptr_t)ptr[j] & (intptr_t)(PAGE_SIZE - 1)) == 0, "Funkcja heap_malloc_aligned() powinien zwrócić adres zaczynający się na początku strony");
                           is_allocated++;
                           break;
                         }
                     }
                     else if (rand_value < 40)
                     {
                       for (int j = 0; j < 5955; ++j)
                         if (ptr_state[j] == 0)
                         {
                           ptr_state[j] = 1;
                           ptr[j] = heap_realloc(NULL, rand() % 1000 + 500);  
                           test_error((intptr_t)ptr[j] % sizeof(void *) == 0, "Funkcja heap_realloc() powinien zwrócić adres będący wielokrotnością długości słowa maszynowego");
                           is_allocated++;
                           break;
                         }
                     }
                     else if (rand_value < 50)
                     {
                       for (int j = 0; j < 5955; ++j)
                         if (ptr_state[j] == 0)
                         {
                           ptr_state[j] = 1;
                           ptr[j] = heap_calloc(rand() % 1000 + 500, rand() % 4 + 1);  
                           test_error((intptr_t)ptr[j] % sizeof(void *) == 0, "Funkcja heap_calloc() powinien zwrócić adres będący wielokrotnością długości słowa maszynowego");
                           is_allocated++;
                           break;
                         }
                     }
                     else if (rand_value < 60)
                     {
                       for (int j = 0; j < 5955; ++j)
                         if (ptr_state[j] == 0)
                         {
                           ptr_state[j] = 1;
                           ptr[j] = heap_malloc(rand() % 1000 + 500);  
                           test_error((intptr_t)ptr[j] % sizeof(void *) == 0, "Funkcja heap_malloc() powinien zwrócić adres będący wielokrotnością długości słowa maszynowego");
                           is_allocated++;
                           break;
                         }
                     }
                     else if (rand_value < 70)
                     {
                       if (is_allocated)
                       {
                          int to_reallocate = rand() % is_allocated;
                          for (int j = 0; j < 5955; ++j)
                           {
                               if (ptr_state[j] == 1 && !to_reallocate)
                               {
                                   if (rand() % 100 < 50)
                                       ptr[j] = heap_realloc(ptr[j], rand() % 100 + 50);
                                   else
                                       ptr[j] = heap_realloc(ptr[j], rand() % 1000 + 500);    
                                   test_error((intptr_t)ptr[j] % sizeof(void *) == 0, "Funkcja heap_malloc() powinien zwrócić adres będący wielokrotnością długości słowa maszynowego");
                                   break;
                               }
                               to_reallocate--;
                           }
                       }
                     }
                     else if (rand_value < 80)
                     {
                       if (is_allocated)
                       {
                         int to_free = rand() % is_allocated;
                          for (int j = 0; j < 5955; ++j)
                           {
                               if (ptr_state[j] == 1 && !to_free)
                               {
                                 ptr_state[j] = 0;
                                 is_allocated--;
                                 heap_realloc_aligned(ptr[j], 0);
                                 break;
                               }
                               to_free--;
                           }
                       }
                     }
                     else if (rand_value < 90)
                     {
                       if (is_allocated)
                       {
                         int to_free = rand() % is_allocated;
                          for (int j = 0; j < 5955; ++j)
                           {
                               if (ptr_state[j] == 1 && !to_free)
                               {
                                 ptr_state[j] = 0;
                                 is_allocated--;
                                 heap_realloc(ptr[j], 0);
                                 break;
                               }
                               to_free--;
                           }
                       }
                     }
                     else 
                     {
                       if (is_allocated)
                       {
                         int to_free = rand() % is_allocated;
                          for (int j = 0; j < 5955; ++j)
                           {
                               if (ptr_state[j] == 1 && !to_free)
                               {
                                 ptr_state[j] = 0;
                                 is_allocated--;
                                 heap_free(ptr[j]);
                                 break;
                               }
                               to_free--;
                           }
                       }
                     }

                     status = heap_validate();
                     test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);           
                   }

                   for (int j = 0; j < 5955; ++j)
                     if (ptr_state[j] == 1)
                         heap_realloc_aligned(ptr[j], 0);

                   test_error(heap_get_largest_used_block_size() == 0, "Funkcja heap_get_largest_used_block_size() powinna zwrócić wartość 0, a zwróciła na %llu", heap_get_largest_used_block_size());

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);

             
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 136: Sprawdzanie poprawności działania funkcji wszystkich funkcji z rozszerzeniem debug alokujących pamięć
//
void UTEST136(void)
{
    // informacje o teście
    test_start(136, "Sprawdzanie poprawności działania funkcji wszystkich funkcji z rozszerzeniem debug alokujących pamięć", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                   srand (time(NULL));

                   int status = heap_setup();
                   test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                   char *ptr[9814];
                   int ptr_state[9814] = {0};

                   int is_allocated = 0;

                   for (int i = 0; i < 9814; ++i)
                   {
                     int rand_value = rand() % 100;
                     if (rand_value < 10)
                     {
                       for (int j = 0; j < 9814; ++j)
                         if (ptr_state[j] == 0)
                         {
                           ptr_state[j] = 1;
                           ptr[j] = heap_realloc_aligned_debug(NULL, rand() % 1000 + 500, __LINE__, __FILE__);  
                           test_error(((intptr_t)ptr[j] & (intptr_t)(PAGE_SIZE - 1)) == 0, "Funkcja heap_realloc_aligned_debug() powinien zwrócić adres zaczynający się na początku strony");
                           is_allocated++;
                           break;
                         }
                     }
                     else if (rand_value < 20)
                     {
                       for (int j = 0; j < 9814; ++j)
                         if (ptr_state[j] == 0)
                         {
                           ptr_state[j] = 1;
                           ptr[j] = heap_calloc_aligned_debug(rand() % 1000 + 500, rand() % 4 + 1, __LINE__, __FILE__);  
                           test_error(((intptr_t)ptr[j] & (intptr_t)(PAGE_SIZE - 1)) == 0, "Funkcja heap_calloc_aligned_debug() powinien zwrócić adres zaczynający się na początku strony");
                           is_allocated++;
                           break;
                         }
                     }
                     else if (rand_value < 30)
                     {
                       for (int j = 0; j < 9814; ++j)
                         if (ptr_state[j] == 0)
                         {
                           ptr_state[j] = 1;
                           ptr[j] = heap_malloc_aligned_debug(rand() % 1000 + 500, __LINE__, __FILE__);  
                           test_error(((intptr_t)ptr[j] & (intptr_t)(PAGE_SIZE - 1)) == 0, "Funkcja heap_malloc_aligned_debug() powinien zwrócić adres zaczynający się na początku strony");
                           is_allocated++;
                           break;
                         }
                     }
                     else if (rand_value < 40)
                     {
                       for (int j = 0; j < 9814; ++j)
                         if (ptr_state[j] == 0)
                         {
                           ptr_state[j] = 1;
                           ptr[j] = heap_realloc_debug(NULL, rand() % 1000 + 500, __LINE__, __FILE__);  
                           test_error((intptr_t)ptr[j] % sizeof(void *) == 0, "Funkcja heap_realloc_debug() powinien zwrócić adres będący wielokrotnością długości słowa maszynowego");
                           is_allocated++;
                           break;
                         }
                     }
                     else if (rand_value < 50)
                     {
                       for (int j = 0; j < 9814; ++j)
                         if (ptr_state[j] == 0)
                         {
                           ptr_state[j] = 1;
                           ptr[j] = heap_calloc_debug(rand() % 1000 + 500, rand() % 4 + 1, __LINE__, __FILE__);  
                           test_error((intptr_t)ptr[j] % sizeof(void *) == 0, "Funkcja heap_calloc_debug() powinien zwrócić adres będący wielokrotnością długości słowa maszynowego");
                           is_allocated++;
                           break;
                         }
                     }
                     else if (rand_value < 60)
                     {
                       for (int j = 0; j < 9814; ++j)
                         if (ptr_state[j] == 0)
                         {
                           ptr_state[j] = 1;
                           ptr[j] = heap_malloc_debug(rand() % 1000 + 500, __LINE__, __FILE__);  
                           test_error((intptr_t)ptr[j] % sizeof(void *) == 0, "Funkcja heap_malloc_debug() powinien zwrócić adres będący wielokrotnością długości słowa maszynowego");
                           is_allocated++;
                           break;
                         }
                     }
                     else if (rand_value < 70)
                     {
                       if (is_allocated)
                       {
                          int to_reallocate = rand() % is_allocated;
                          for (int j = 0; j < 9814; ++j)
                           {
                               if (ptr_state[j] == 1 && !to_reallocate)
                               {
                                   if (rand() % 100 < 50)
                                       ptr[j] = heap_realloc_debug(ptr[j], rand() % 100 + 50, __LINE__, __FILE__);
                                   else
                                       ptr[j] = heap_realloc_debug(ptr[j], rand() % 1000 + 500, __LINE__, __FILE__);    
                                   test_error((intptr_t)ptr[j] % sizeof(void *) == 0, "Funkcja heap_realloc_debug() powinien zwrócić adres będący wielokrotnością długości słowa maszynowego");
                                   break;
                               }
                               to_reallocate--;
                           }
                       }
                     }
                     else if (rand_value < 80)
                     {
                       if (is_allocated)
                       {
                         int to_free = rand() % is_allocated;
                          for (int j = 0; j < 9814; ++j)
                           {
                               if (ptr_state[j] == 1 && !to_free)
                               {
                                 ptr_state[j] = 0;
                                 is_allocated--;
                                 heap_realloc_aligned_debug(ptr[j], 0, __LINE__, __FILE__);
                                 break;
                               }
                               to_free--;
                           }
                       }
                     }
                     else if (rand_value < 90)
                     {
                       if (is_allocated)
                       {
                         int to_free = rand() % is_allocated;
                          for (int j = 0; j < 9814; ++j)
                           {
                               if (ptr_state[j] == 1 && !to_free)
                               {
                                 ptr_state[j] = 0;
                                 is_allocated--;
                                 heap_realloc_debug(ptr[j], 0, __LINE__, __FILE__);
                                 break;
                               }
                               to_free--;
                           }
                       }
                     }
                     else 
                     {
                       if (is_allocated)
                       {
                         int to_free = rand() % is_allocated;
                          for (int j = 0; j < 9814; ++j)
                           {
                               if (ptr_state[j] == 1 && !to_free)
                               {
                                 ptr_state[j] = 0;
                                 is_allocated--;
                                 heap_free(ptr[j]);
                                 break;
                               }
                               to_free--;
                           }
                       }
                     }

                     status = heap_validate();
                     test_error(status == 0, "Funkcja heap_validate() powinna zwrócić wartość 0, a zwróciła na %d", status);           
                   }

                   for (int j = 0; j < 9814; ++j)
                     if (ptr_state[j] == 1)
                         heap_realloc_aligned(ptr[j], 0);

                   test_error(heap_get_largest_used_block_size() == 0, "Funkcja heap_get_largest_used_block_size() powinna zwrócić wartość 0, a zwróciła na %llu", heap_get_largest_used_block_size());

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);

             
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 137: Sprawdzanie poprawności działania funkcji malloc i free w przypadku wywoływania ich w różnych wątkach
//
void UTEST137(void)
{
    // informacje o teście
    test_start(137, "Sprawdzanie poprawności działania funkcji malloc i free w przypadku wywoływania ich w różnych wątkach", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                srand (time(NULL));
                
                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);
            
                pthread_t threads[19];

                for (int i = 0; i < 19; ++i)
                    pthread_create(threads + i, NULL, heap_malloc_test_thread, NULL);
            
            
                for (int i = 0; i < 19; ++i)
                    pthread_join(threads[i], NULL);
                
                status = custom_sbrk_check_fences_integrity();
                test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);
                
                heap_clean();
                
                uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);

             
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 138: Sprawdzanie poprawności działania funkcji *_aligned i free w przypadku wywoływania ich w różnych wątkach
//
void UTEST138(void)
{
    // informacje o teście
    test_start(138, "Sprawdzanie poprawności działania funkcji *_aligned i free w przypadku wywoływania ich w różnych wątkach", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                srand (time(NULL));

                int status = heap_setup();
                test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                pthread_t threads[19];

                for (int i = 0; i < 19; ++i)
                    pthread_create(threads + i, NULL, heap_alloc_aligned_test_thread, NULL);


                for (int i = 0; i < 19; ++i)
                    pthread_join(threads[i], NULL);

                status = custom_sbrk_check_fences_integrity();
                test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                heap_clean();

                uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);

             
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 139: Sprawdzanie poprawności działania wszystkich funkcji w przypadku wywoływania ich w różnych wątkach
//
void UTEST139(void)
{
    // informacje o teście
    test_start(139, "Sprawdzanie poprawności działania wszystkich funkcji w przypadku wywoływania ich w różnych wątkach", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    
                 srand (time(NULL));

                 int status = heap_setup();
                 test_error(status == 0, "Funkcja heap_setup() powinna zwrócić wartość 0, a zwróciła na %d", status);

                 pthread_t threads[17];

                 for (int i = 0; i < 17; ++i)
                     pthread_create(threads + i, NULL, heap_alloc_ultimate_test_thread, NULL);


                 for (int i = 0; i < 17; ++i)
                     pthread_join(threads[i], NULL);

                 status = custom_sbrk_check_fences_integrity();
                 test_error(status == 0, "Funkcja custom_sbrk_check_fences_integrity() powinna zwrócić wartość 0, a zwróciła na %d. Oznacza to, że alokator nadpisał pamięć, która nie została przydzielona przez system", status);

                 heap_clean();

                 uint64_t reserved_memory = custom_sbrk_get_reserved_memory();
                 test_error(reserved_memory == 0, "Funkcja custom_sbrk_get_reserved_memory() powinna zwrócić wartość 0, a zwróciła na %llu. Po wywołaniu funkcji heap_clean cała pamięć zarezerwowana przez alokator powinna być zwrócona do systemu", reserved_memory);

              
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}




enum run_mode_t { rm_normal_with_rld = 0, rm_unit_test = 1, rm_main_test = 2 };

int __wrap_main(volatile int _argc, char** _argv, char** _envp)
{
    int volatile vargc = _argc;
    char ** volatile vargv = _argv, ** volatile venvp = _envp;
	volatile enum run_mode_t run_mode = rm_unit_test; // -1
	volatile int selected_test = -1;

    if (vargc > 1)
	{
	    char* smode = strtok(vargv[1], ",");
	    char* stest = strtok(NULL, "");
		char *errptr = NULL;
		run_mode = (enum run_mode_t)strtol(smode, &errptr, 10);
		if (*errptr == '\x0')
		{
			memmove(vargv + 1, vargv + 2, sizeof(char*) * (vargc - 1));
			vargc--;

			if (stest != NULL)
			{
			    int val = (int)strtol(stest, &errptr, 10);
			    if (*errptr == '\x0')
			        selected_test = val;
			}
		}
	}

    // printf("runmode=%d; selected_test=%d\n", run_mode, selected_test);

    // inicjuj testy jednostkowe
    unit_test_init(run_mode, "unit_test_v2.c");
    test_limit_init();
    rldebug_set_reported_severity_level(MSL_FAILURE);

    if (run_mode == rm_normal_with_rld)
    {
        // konfiguracja ograniczników
        rldebug_reset_limits();
        

        // uruchom funkcję main Studenta a potem wyświetl podsumowanie sterty i zasobów
        volatile int ret_code = rdebug_call_main(tested_main, vargc, vargv, venvp);

        rldebug_reset_limits();
        

        int leaks_detected = rldebug_show_leaked_resources(0);
        if (leaks_detected)
            raise(SIGHEAP);

        return ret_code;
    }

    
    if (run_mode == rm_unit_test)
    {
        test_title("Testy jednostkowe");

        void (*pfcn[])(void) =
        { 
            UTEST1, // Sprawdzanie poprawności działania funkcji heap_malloc - test sprawdza poprawność działania funkcji w przypadku przekazania do niej wartości 0
            UTEST2, // Sprawdzanie poprawności działania funkcji heap_malloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni większej niż przestrzeń obecna na stercie
            UTEST3, // Sprawdzanie poprawności działania funkcji heap_malloc - test sprawdza poprawność działania funkcji w przypadku pierwszej próby zaalokowania przestrzeni mniejszej niż przestrzeń obecna na stercie
            UTEST4, // Sprawdzanie poprawności działania funkcji heap_malloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni mniejszej niż przestrzeń obecna na stercie, w przypadku przydzielenia wcześniej pamięci użytkownikowi
            UTEST5, // Sprawdzanie poprawności działania funkcji heap_malloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni mniejszej niż przestrzeń obecna na stercie (żądana pamięć wymaga zwiększenia limitu pamięci przydzielonego przez system), w przypadku przydzielenia wcześniej pamięci użytkownikowi
            UTEST6, // Sprawdzanie poprawności działania funkcji heap_malloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni większej niż przestrzeń, która może być przydzielona przez system (żądana pamięć wymaga zwiększenia limitu pamięci przydzielonego przez system), w przypadku przydzielenia wcześniej pamięci użytkownikowi
            UTEST7, // Sprawdzanie poprawności działania funkcji heap_malloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeń mniejszej niż przestrzeń poprzednio zwolniona
            UTEST8, // Sprawdzanie poprawności działania funkcji heap_malloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni równej przestrzeni poprzednio zwolnionej
            UTEST9, // Sprawdzanie poprawności działania funkcji heap_malloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni mniejszej niż przestrzeń poprzednio zwolniona
            UTEST10, // Sprawdzanie poprawności działania funkcji heap_malloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni równej przestrzeń poprzednio zwolnionej
            UTEST11, // Sprawdzanie poprawności działania funkcji heap_malloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni większej niż przestrzeń poprzednio zwolnionej
            UTEST12, // Sprawdzanie poprawności działania funkcji heap_malloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni większej niż przestrzeń poprzednio zwolnionej
            UTEST13, // Sprawdzanie poprawności działania funkcji heap_malloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni większej niż przestrzeń poprzednio zwolnionej
            UTEST14, // Sprawdzanie poprawności działania funkcji heap_calloc - test sprawdza poprawność działania funkcji w przypadku przekazania do niej wartości 0
            UTEST15, // Sprawdzanie poprawności działania funkcji heap_calloc - test sprawdza poprawność działania funkcji w przypadku przekazania do niej wartości 0
            UTEST16, // Sprawdzanie poprawności działania funkcji heap_calloc - test sprawdza poprawność działania funkcji w przypadku przekazania do niej wartości 0
            UTEST17, // Sprawdzanie poprawności działania funkcji heap_calloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni większej niż przestrzeń obecna na stercie
            UTEST18, // Sprawdzanie poprawności działania funkcji heap_calloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni większej niż przestrzeń obecna na stercie
            UTEST19, // Sprawdzanie poprawności działania funkcji heap_calloc - test sprawdza poprawność działania funkcji w przypadku pierwszej próby zaalokowania przestrzeni mniejszej niż przestrzeń obecna na stercie
            UTEST20, // Sprawdzanie poprawności działania funkcji heap_calloc - test sprawdza poprawność działania funkcji w przypadku pierwszej próby zaalokowania przestrzeni mniejszej niż przestrzeń obecna na stercie
            UTEST21, // Sprawdzanie poprawności działania funkcji heap_calloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni mniejszej niż przestrzeń obecna na stercie, w przypadku przydzielenia wcześniej pamięci użytkownikowi
            UTEST22, // Sprawdzanie poprawności działania funkcji heap_calloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni mniejszej niż przestrzeń obecna na stercie (żądana pamięć wymaga zwiększenia limitu pamięci przydzielonego przez system), w przypadku przydzielenia wcześniej pamięci użytkownikowi
            UTEST23, // Sprawdzanie poprawności działania funkcji heap_calloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni większej niż przestrzeń, która może być przydzielona przez system (żądana pamięć wymaga zwiększenia limitu pamięci przydzielonego przez system), w przypadku przydzielenia wcześniej pamięci użytkownikowi
            UTEST24, // Sprawdzanie poprawności działania funkcji heap_calloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeń mniejszej niż przestrzeń poprzednio zwolniona
            UTEST25, // Sprawdzanie poprawności działania funkcji heap_calloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni równej przestrzeni poprzednio zwolnionej
            UTEST26, // Sprawdzanie poprawności działania funkcji heap_calloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni mniejszej niż przestrzeń poprzednio zwolniona
            UTEST27, // Sprawdzanie poprawności działania funkcji heap_calloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni równej przestrzeni poprzednio zwolnionej
            UTEST28, // Sprawdzanie poprawności działania funkcji heap_calloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni większej niż przestrzeń poprzednio zwolnionej
            UTEST29, // Sprawdzanie poprawności działania funkcji heap_calloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni większej niż przestrzeń poprzednio zwolnionej
            UTEST30, // Sprawdzanie poprawności działania funkcji heap_get_largest_used_block_size
            UTEST31, // Sprawdzanie poprawności działania funkcji heap_get_largest_used_block_size
            UTEST32, // Sprawdzanie poprawności działania funkcji heap_get_largest_used_block_size
            UTEST33, // Sprawdzanie poprawności działania funkcji heap_get_largest_used_block_size
            UTEST34, // Sprawdzanie poprawności działania funkcji heap_get_largest_used_block_size
            UTEST35, // Sprawdzanie poprawności działania funkcji heap_free - test sprawdza poprawność działania funkcji w przypadku przekazania do niej niepoprawnego wskaźnika lub wskaźnika sprzed inicjalizacji sterty
            UTEST36, // Sprawdzanie poprawności działania funkcji heap_free - test sprawdza poprawność działania funkcji w przypadku przekazania do niej niepoprawnego wskaźnika
            UTEST37, // Sprawdzanie poprawności działania funkcji heap_free - test sprawdza poprawność działania funkcji w przypadku przekazania do niej niepoprawnego wskaźnika
            UTEST38, // Sprawdzanie poprawności działania funkcji heap_free - test sprawdza poprawność działania funkcji w przypadku przekazania do niej niepoprawnego wskaźnika
            UTEST39, // Sprawdzanie poprawności działania funkcji heap_free - test sprawdza poprawność działania funkcji w przypadku przekazania do niej niepoprawnego wskaźnika
            UTEST40, // Sprawdzanie poprawności działania funkcji heap_free - test sprawdza poprawność działania funkcji w przypadku zwolnienia dwóch bloków pamięci sąsiadujących ze sobą
            UTEST41, // Sprawdzanie poprawności działania funkcji heap_free - test sprawdza poprawność działania funkcji w przypadku zwolnienia dwóch bloków pamięci sąsiadujących ze sobą i zaalokowanie w tym miejscu nowej pamięci o rozmiarze sumy dwóch zwolnionych bloków
            UTEST42, // Sprawdzanie poprawności działania funkcji heap_free - test sprawdza poprawność działania funkcji w przypadku zwolnienia dwóch bloków pamięci sąsiadujących ze sobą
            UTEST43, // Sprawdzanie poprawności działania funkcji heap_free - test sprawdza poprawność działania funkcji w przypadku zwolnienia dwóch bloków pamięci sąsiadujących ze sobą i zaalokowanie w tym miejscu nowej pamięci o rozmiarze sumy dwóch zwolnionych bloków
            UTEST44, // Sprawdzanie poprawności działania funkcji heap_free - test sprawdza poprawność działania funkcji w przypadku zwolnienia trzech bloków pamięci sąsiadujących ze sobą
            UTEST45, // Sprawdzanie poprawności działania funkcji heap_free - test sprawdza poprawność działania funkcji w przypadku zwolnienia trzech bloków pamięci sąsiadujących ze sobą i zaalokowanie w tym miejscu nowej pamięci o rozmiarze sumy dwóch zwolnionych bloków
            UTEST46, // Sprawdzanie poprawności działania funkcji heap_free - test sprawdza poprawność działania funkcji w przypadku zwolnienia bloku pamięci, zaalokowania w to miejsce mniejszego bloku, zwolnienia go i zaalokowania ponownie większego bloku
            UTEST47, // Sprawdzanie poprawności działania funkcji heap_free - test sprawdza poprawność działania funkcji w przypadku zwolnienia trzech bloków pamięci sąsiadujących ze sobą i zaalokowanie w tym miejscu nowej pamięci o rozmiarze sumy dwóch zwolnionych bloków
            UTEST48, // Sprawdzanie poprawności działania funkcji heap_free - test sprawdza poprawność działania funkcji w przypadku zwolnienia trzech bloków pamięci sąsiadujących ze sobą i zaalokowanie w tym miejscu nowej pamięci o rozmiarze sumy dwóch zwolnionych bloków
            UTEST49, // Sprawdzanie poprawności działania funkcji heap_malloc, heap_calloc i heap_free - test sprawdza poprawność działania funkcji w przypadku zwolnienia trzech bloków pamięci sąsiadujących ze sobą i zaalokowanie w tym miejscu nowej pamięci o rozmiarze sumy dwóch zwolnionych bloków
            UTEST50, // Sprawdzanie poprawności działania funkcji get_pointer_type
            UTEST51, // Sprawdzanie poprawności działania funkcji get_pointer_type
            UTEST52, // Sprawdzanie poprawności działania funkcji get_pointer_type
            UTEST53, // Sprawdzanie poprawności działania funkcji get_pointer_type
            UTEST54, // Sprawdzanie poprawności działania funkcji get_pointer_type
            UTEST55, // Sprawdzanie poprawności działania funkcji get_pointer_type
            UTEST56, // Sprawdzanie poprawności działania funkcji get_pointer_type
            UTEST57, // Sprawdzanie poprawności działania funkcji get_pointer_type
            UTEST58, // Sprawdzanie poprawności działania funkcji get_pointer_type
            UTEST59, // Sprawdzanie poprawności działania funkcji get_pointer_type
            UTEST60, // Sprawdzanie poprawności działania funkcji get_pointer_type
            UTEST61, // Sprawdzanie poprawności działania funkcji get_pointer_type
            UTEST62, // Sprawdzanie poprawności działania funkcji get_pointer_type
            UTEST63, // Sprawdzanie poprawności działania funkcji get_pointer_type
            UTEST64, // Sprawdzanie poprawności działania funkcji heap_validate
            UTEST65, // Sprawdzanie poprawności działania funkcji heap_validate
            UTEST66, // Sprawdzanie poprawności działania funkcji heap_validate
            UTEST67, // Sprawdzanie poprawności działania funkcji heap_validate
            UTEST68, // Sprawdzanie poprawności działania funkcji heap_validate
            UTEST69, // Sprawdzanie poprawności działania funkcji heap_validate
            UTEST70, // Sprawdzanie poprawności działania funkcji heap_validate
            UTEST71, // Sprawdzanie poprawności działania funkcji heap_validate
            UTEST72, // Sprawdzanie poprawności działania funkcji heap_realloc - test sprawdza poprawność działania funkcji w przypadku przekazania do niej wartości 0
            UTEST73, // Sprawdzanie poprawności działania funkcji heap_realloc - test sprawdza poprawność działania funkcji w przypadku niezainicjowania sterty
            UTEST74, // Sprawdzanie poprawności działania funkcji heap_realloc - test sprawdza poprawność działania funkcji w przypadku przekazania do niej poprawnej wartości
            UTEST75, // Sprawdzanie poprawności działania funkcji heap_realloc - test sprawdza poprawność działania funkcji w przypadku przekazania do niej wartości 0
            UTEST76, // Sprawdzanie poprawności działania funkcji heap_realloc - test sprawdza poprawność działania funkcji w przypadku przekazania do niej niewłaściwego wskaźnika
            UTEST77, // Sprawdzanie poprawności działania funkcji heap_realloc - test sprawdza poprawność działania funkcji w przypadku żądania zmniejszenia rozmiaru pamięci
            UTEST78, // Sprawdzanie poprawności działania funkcji heap_realloc - test sprawdza poprawność działania funkcji w przypadku żądania tego samego rozmiaru pamięci, który zajmował poprzednio
            UTEST79, // Sprawdzanie poprawności działania funkcji heap_realloc - test sprawdza poprawność działania funkcji w przypadku żądania większego rozmiaru pamięci, który zajmował poprzednio
            UTEST80, // Sprawdzanie poprawności działania funkcji heap_realloc - test sprawdza poprawność działania funkcji w przypadku żądania większego rozmiaru pamięci, który zajmował poprzednio
            UTEST81, // Sprawdzanie poprawności działania funkcji heap_realloc - test sprawdza poprawność działania funkcji w przypadku żądania większego rozmiaru pamięci, który zajmował poprzednio
            UTEST82, // Sprawdzanie poprawności działania funkcji heap_realloc - test sprawdza poprawność działania funkcji w przypadku żądania większego rozmiaru pamięci, który zajmował poprzednio
            UTEST83, // Sprawdzanie poprawności działania funkcji heap_realloc - test sprawdza poprawność działania funkcji w przypadku żądania większego rozmiaru pamięci, który zajmował poprzednio
            UTEST84, // Sprawdzanie poprawności działania funkcji heap_realloc - test sprawdza poprawność działania funkcji w przypadku żądania większego rozmiaru pamięci, który zajmował poprzednio
            UTEST85, // Sprawdzanie poprawności działania funkcji heap_realloc - test sprawdza poprawność działania funkcji w przypadku żądania większego rozmiaru pamięci, który zajmował poprzednio
            UTEST86, // Sprawdzanie poprawności działania funkcji heap_realloc - test sprawdza poprawność działania funkcji w przypadku żądania większego rozmiaru pamięci, który zajmował poprzednio
            UTEST87, // Sprawdzanie poprawności działania funkcji heap_malloc_aligned - test sprawdza poprawność działania funkcji w przypadku przekazania do niej wartości 0
            UTEST88, // Sprawdzanie poprawności działania funkcji heap_malloc_aligned - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni większej niż przestrzeń obecna na stercie
            UTEST89, // Sprawdzanie poprawności działania funkcji heap_malloc_aligned - test sprawdza poprawność działania funkcji w przypadku pierwszej próby zaalokowania przestrzeni mniejszej niż przestrzeń obecna na stercie
            UTEST90, // Sprawdzanie poprawności działania funkcji heap_malloc_aligned - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni mniejszej niż przestrzeń obecna na stercie, w przypadku przydzielenia wcześniej pamięci użytkownikowi
            UTEST91, // Sprawdzanie poprawności działania funkcji heap_malloc_aligned - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni mniejszej niż przestrzeń obecna na stercie (żądana pamięć wymaga zwiększenia limitu pamięci przydzielonego przez system), w przypadku przydzielenia wcześniej pamięci użytkownikowi
            UTEST92, // Sprawdzanie poprawności działania funkcji heap_malloc_aligned - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni większej niż przestrzeń, która może być przydzielona przez system (żądana pamięć wymaga zwiększenia limitu pamięci przydzielonego przez system), w przypadku przydzielenia wcześniej pamięci użytkownikowi
            UTEST93, // Sprawdzanie poprawności działania funkcji heap_malloc_aligned - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeń mniejszej niż przestrzeń poprzednio zwolniona
            UTEST94, // Sprawdzanie poprawności działania funkcji heap_malloc_aligned - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni równej przestrzeni poprzednio zwolnionej
            UTEST95, // Sprawdzanie poprawności działania funkcji heap_malloc_aligned - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni równej przestrzeni poprzednio zwolnionej
            UTEST96, // Sprawdzanie poprawności działania funkcji heap_malloc_aligned - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni równej przestrzeni poprzednio zwolnionej
            UTEST97, // Sprawdzanie poprawności działania funkcji heap_malloc_aligned - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni równej przestrzeni poprzednio zwolnionej
            UTEST98, // Sprawdzanie poprawności działania funkcji heap_malloc_aligned - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni większej niż przestrzeń poprzednio zwolniona
            UTEST99, // Sprawdzanie poprawności działania funkcji heap_malloc_aligned - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni równej przestrzeń poprzednio zwolnionej
            UTEST100, // Sprawdzanie poprawności działania funkcji heap_malloc_aligned - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni większej niż przestrzeń poprzednio zwolnionej
            UTEST101, // Sprawdzanie poprawności działania funkcji heap_malloc_aligned - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni większej niż przestrzeń poprzednio zwolnionej
            UTEST102, // Sprawdzanie poprawności działania funkcji heap_calloc_aligned - test sprawdza poprawność działania funkcji w przypadku przekazania do niej wartości 0
            UTEST103, // Sprawdzanie poprawności działania funkcji heap_calloc_aligned - test sprawdza poprawność działania funkcji w przypadku przekazania do niej wartości 0
            UTEST104, // Sprawdzanie poprawności działania funkcji heap_calloc_aligned - test sprawdza poprawność działania funkcji w przypadku przekazania do niej wartości 0
            UTEST105, // Sprawdzanie poprawności działania funkcji heap_calloc_aligned - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni większej niż przestrzeń obecna na stercie
            UTEST106, // Sprawdzanie poprawności działania funkcji heap_calloc_aligned - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni większej niż przestrzeń obecna na stercie
            UTEST107, // Sprawdzanie poprawności działania funkcji heap_calloc_aligned - test sprawdza poprawność działania funkcji w przypadku pierwszej próby zaalokowania przestrzeni mniejszej niż przestrzeń obecna na stercie
            UTEST108, // Sprawdzanie poprawności działania funkcji heap_calloc_aligned - test sprawdza poprawność działania funkcji w przypadku pierwszej próby zaalokowania przestrzeni mniejszej niż przestrzeń obecna na stercie
            UTEST109, // Sprawdzanie poprawności działania funkcji heap_calloc_aligned - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni mniejszej niż przestrzeń obecna na stercie, w przypadku przydzielenia wcześniej pamięci użytkownikowi
            UTEST110, // Sprawdzanie poprawności działania funkcji heap_calloc_aligned - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni mniejszej niż przestrzeń obecna na stercie (żądana pamięć wymaga zwiększenia limitu pamięci przydzielonego przez system), w przypadku przydzielenia wcześniej pamięci użytkownikowi
            UTEST111, // Sprawdzanie poprawności działania funkcji heap_calloc_aligned - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni większej niż przestrzeń, która może być przydzielona przez system (żądana pamięć wymaga zwiększenia limitu pamięci przydzielonego przez system), w przypadku przydzielenia wcześniej pamięci użytkownikowi
            UTEST112, // Sprawdzanie poprawności działania funkcji heap_calloc - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeń mniejszej niż przestrzeń poprzednio zwolniona
            UTEST113, // Sprawdzanie poprawności działania funkcji heap_calloc_aligned - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni równej przestrzeni poprzednio zwolnionej
            UTEST114, // Sprawdzanie poprawności działania funkcji heap_calloc_aligned - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni równej przestrzeni poprzednio zwolnionej
            UTEST115, // Sprawdzanie poprawności działania funkcji heap_malloc_aligned - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni równej przestrzeni poprzednio zwolnionej
            UTEST116, // Sprawdzanie poprawności działania funkcji heap_calloc_aligned - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni mniejszej niż przestrzeń poprzednio zwolniona
            UTEST117, // Sprawdzanie poprawności działania funkcji heap_calloc_aligned - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni równej przestrzeni poprzednio zwolnionej
            UTEST118, // Sprawdzanie poprawności działania funkcji heap_calloc_aligned - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni większej niż przestrzeń poprzednio zwolnionej
            UTEST119, // Sprawdzanie poprawności działania funkcji heap_calloc_aligned - test sprawdza poprawność działania funkcji w przypadku próby zaalokowania przestrzeni większej niż przestrzeń poprzednio zwolnionej
            UTEST120, // Sprawdzanie poprawności działania funkcji heap_realloc_aligned - test sprawdza poprawność działania funkcji w przypadku przekazania do niej wartości 0
            UTEST121, // Sprawdzanie poprawności działania funkcji heap_realloc_aligned - test sprawdza poprawność działania funkcji w przypadku niezainicjowania sterty
            UTEST122, // Sprawdzanie poprawności działania funkcji heap_realloc_aligned - test sprawdza poprawność działania funkcji w przypadku przekazania do niej poprawnej wartości
            UTEST123, // Sprawdzanie poprawności działania funkcji heap_realloc_aligned - test sprawdza poprawność działania funkcji w przypadku przekazania do niej wartości 0
            UTEST124, // Sprawdzanie poprawności działania funkcji heap_realloc_aligned - test sprawdza poprawność działania funkcji w przypadku przekazania do niej niewłaściwego wskaźnika
            UTEST125, // Sprawdzanie poprawności działania funkcji heap_realloc_aligned - test sprawdza poprawność działania funkcji w przypadku żądania zmniejszenia rozmiaru pamięci
            UTEST126, // Sprawdzanie poprawności działania funkcji heap_realloc_aligned - test sprawdza poprawność działania funkcji w przypadku żądania tego samego rozmiaru pamięci, który zajmował poprzednio
            UTEST127, // Sprawdzanie poprawności działania funkcji heap_realloc_aligned - test sprawdza poprawność działania funkcji w przypadku żądania większego rozmiaru pamięci, który zajmował poprzednio
            UTEST128, // Sprawdzanie poprawności działania funkcji heap_realloc_aligned - test sprawdza poprawność działania funkcji w przypadku żądania większego rozmiaru pamięci, który zajmował poprzednio
            UTEST129, // Sprawdzanie poprawności działania funkcji heap_realloc_aligned - test sprawdza poprawność działania funkcji w przypadku żądania większego rozmiaru pamięci, który zajmował poprzednio
            UTEST130, // Sprawdzanie poprawności działania funkcji heap_realloc_aligned - test sprawdza poprawność działania funkcji w przypadku żądania większego rozmiaru pamięci, który zajmował poprzednio
            UTEST131, // Sprawdzanie poprawności działania funkcji heap_realloc_aligned - test sprawdza poprawność działania funkcji w przypadku żądania większego rozmiaru pamięci, który zajmował poprzednio
            UTEST132, // Sprawdzanie poprawności działania funkcji heap_realloc_aligned - test sprawdza poprawność działania funkcji w przypadku żądania większego rozmiaru pamięci, który zajmował poprzednio
            UTEST133, // Sprawdzanie poprawności działania funkcji heap_realloc_aligned - test sprawdza poprawność działania funkcji w przypadku żądania większego rozmiaru pamięci, który zajmował poprzednio
            UTEST134, // Sprawdzanie poprawności działania funkcji heap_realloc_aligned - test sprawdza poprawność kolejności przydzielania bloków pamięci
            UTEST135, // Sprawdzanie poprawności działania funkcji wszystkich funkcji alokujących pamięć
            UTEST136, // Sprawdzanie poprawności działania funkcji wszystkich funkcji z rozszerzeniem debug alokujących pamięć
            UTEST137, // Sprawdzanie poprawności działania funkcji malloc i free w przypadku wywoływania ich w różnych wątkach
            UTEST138, // Sprawdzanie poprawności działania funkcji *_aligned i free w przypadku wywoływania ich w różnych wątkach
            UTEST139, // Sprawdzanie poprawności działania wszystkich funkcji w przypadku wywoływania ich w różnych wątkach
            NULL
        };

        for (int idx = 0; pfcn[idx] != NULL && !test_get_session_termination_flag(); idx++)
        {
            if (selected_test == -1 || selected_test == idx + 1)
                pfcn[idx]();

            // limit niezaliczonych testów, po jakim testy jednostkowe zostaną przerwane
            if (test_session_get_fail_count() >= 1000)
                test_terminate_session();
        }


        test_title("RLDebug :: Analiza wycieku zasobów");
        // sprawdź wycieki pamięci
        int leaks_detected = rldebug_show_leaked_resources(1);
        test_set_session_leaks(leaks_detected);

        // poinformuj serwer Mrówka o wyniku testu - podsumowanie
        test_title("Podsumowanie");
        if (selected_test == -1)
            test_summary(139); // wszystkie testy muszą zakończyć się sukcesem
        else
            test_summary(1); // tylko jeden (selected_test) test musi zakończyć się  sukcesem
        return EXIT_SUCCESS;
    }
    

    if (run_mode == rm_main_test)
    {
        test_title("Testy funkcji main()");

        void (*pfcn[])(int, char**, char**) =
        { 
            NULL
        };

        for (volatile int idx = 0; pfcn[idx] != NULL && !test_get_session_termination_flag(); idx++)
        {
            if (selected_test == -1 || selected_test == idx + 1)
                pfcn[idx](vargc, vargv, venvp);

            // limit niezaliczonych testów, po jakim testy jednostkowe zostaną przerwane
            if (test_session_get_fail_count() >= 1000)
                test_terminate_session();
        }


        test_title("RLDebug :: Analiza wycieku zasobów");
        // sprawdź wycieki pamięci
        int leaks_detected = rldebug_show_leaked_resources(1);
        test_set_session_leaks(leaks_detected);

        // poinformuj serwer Mrówka o wyniku testu - podsumowanie
        test_title("Podsumowanie");
        if (selected_test == -1)
            test_summary(0); // wszystkie testy muszą zakończyć się sukcesem
        else
            test_summary(1); // tylko jeden (selected_test) test musi zakończyć się  sukcesem

        return EXIT_SUCCESS;
    }

    printf("*** Nieznana wartość RunMode: %d", (int)run_mode);
    abort();
}
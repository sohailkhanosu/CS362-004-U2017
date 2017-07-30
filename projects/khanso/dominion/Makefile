CFLAGS = -std=c99 -Wall -fpic -coverage -lm

CARD_TESTS  = $(wildcard cardtest*.c)
UNIT_TESTS = $(wildcard unittest*.c)

CARD_TESTS_EXES = $(patsubst %.c, %, ${CARD_TESTS})
UNIT_TESTS_EXES = $(patsubst %.c, %, ${UNIT_TESTS})
RANDOM_TESTERS = $(wildcard randomtest*.c)
RANDOM_TESTERS_EXE = $(patsubst %.c, %, ${RANDOM_TESTERS})
RANDOM_TESTERS_OUT = $(patsubst %.c, %.out, ${RANDOM_TESTERS})

TESTS_OUT = unittestresults.out

QUIET=

ifeq (${QUIET}, 1)
	CFLAGS += -DQUIET
endif

define TEST_STR
	echo "TEST RUN: ${test}" | tee -a ${TESTS_OUT};
	@-./${test}  2>&1 | tee -a ${TESTS_OUT};
	@printf "\nGCOV RESULTS AFTER ${test}\n" >> ${TESTS_OUT};
	@gcov -b -c ./dominion.c >> ${TESTS_OUT};
	@echo "" >> ${TESTS_OUT};
endef

${TESTS_OUT}: clean ${UNIT_TESTS_EXES} ${CARD_TESTS_EXES}
	@rm -f ${TESTS_OUT}
	$(foreach test, $(sort ${CARD_TESTS_EXES}), $(TEST_STR))
	$(foreach test, $(sort ${UNIT_TESTS_EXES}), $(TEST_STR))
	@printf "\n GCOV SUMMARY\n" >> ${TESTS_OUT}
	@cat ./dominion.c.gcov >> ${TESTS_OUT}

${UNIT_TESTS_EXES}: % : %.c dominion.o rngs.o
	gcc -o $@ -g $^ $(CFLAGS)

${CARD_TESTS_EXES}: % : %.c dominion.o rngs.o
	gcc -o $@ -g $^ $(CFLAGS)


${RANDOM_TESTERS_OUT}: %.out: %
	@echo "Running Random Test $@" | tee $@
	@ ./$< 2>&1 | tee -a $@
	@printf "\nGCOV results\n" >> $@
	@gcov -b -c ./dominion.c >> $@
	@cat dominion.c.gcov >> $@


${RANDOM_TESTERS_EXE}: % : %.c dominion.o rngs.o random_tester_helper.h
	gcc -o $@ -g $< dominion.o rngs.o ${CFLAGS}

rngs.o: rngs.h rngs.c
	gcc -c rngs.c -g  $(CFLAGS)

dominion.o: dominion.h dominion.c rngs.o
	gcc -c dominion.c -g  $(CFLAGS)

playdom: dominion.o playdom.c
	gcc -o playdom playdom.c -g dominion.o rngs.o $(CFLAGS)
#To run playdom you need to entere: ./playdom <any integer number> like ./playdom 10*/
testDrawCard: testDrawCard.c dominion.o rngs.o
	gcc  -o testDrawCard -g  testDrawCard.c dominion.o rngs.o $(CFLAGS)

badTestDrawCard: badTestDrawCard.c dominion.o rngs.o
	gcc -o badTestDrawCard -g  badTestDrawCard.c dominion.o rngs.o $(CFLAGS)

testBuyCard: testDrawCard.c dominion.o rngs.o
	gcc -o testDrawCard -g  testDrawCard.c dominion.o rngs.o $(CFLAGS)

testAll: dominion.o testSuite.c
	gcc -o testSuite testSuite.c -g  dominion.o rngs.o $(CFLAGS)

interface.o: interface.h interface.c
	gcc -c interface.c -g  $(CFLAGS)

runtests: testDrawCard 
	./testDrawCard &> unittestresult.out
	gcov dominion.c >> unittestresult.out
	cat dominion.c.gcov >> unittestresult.out


player: player.c interface.o
	gcc -o player player.c -g  dominion.o rngs.o interface.o $(CFLAGS)



all: playdom player testDrawCard testBuyCard badTestDrawCard

clean:
	rm -rf *.o playdom.exe playdom player player.exe  *.gcov *.gcda *.gcno *.so *.out testDrawCard testDrawCard.exe ${UNIT_TESTS_EXES} ${CARD_TESTS_EXES} *.dSYM ${RANDOM_TESTERS_EXE}

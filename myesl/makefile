ESLPATH=/home/jgb/freeswitch/libs/esl
CFLAG= -I${ESLPATH}/src/include

all: myesl.c ${ESLPATH}/libesl.a
	gcc ${CFLAG} -o myesl myesl.c ${ESLPATH}/libesl.a -lpthread -lm
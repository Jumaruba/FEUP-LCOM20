PROG=proj

SRCS = proj.c keyboard.c graphic.c sprite.c timer.c utils.c mouse.c menu.c game.c rtc.c textInput.c

# __LCOM_OPTIMIZED__ disables most of the LCF's overhead (and functionality!)
CPPFLAGS += -pedantic #-D __LCOM_OPTIMIZED__

DPADD += ${LIBLCF}
LDADD += -llcf

.include <minix.lcom.mk>
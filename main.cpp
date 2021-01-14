#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}


/*
 Tester powienien (przy wykorzystaniu jednego interfejsu):
 - wykonywać paragony naprzemiennie z fakturami (maksymalna ilość pozycji) w siedmiu stawkach,
 - towary powinny być sprzedawane tak aby od razu się blokowały (po drugim ich wystąpieniu na paragonie),
 - już w trakcie paragonu/faktury program ma odpytywać urządzenie o towary które właśnie się zablokowały,
 W logu:
 - to co wysłane,
 - czasy odpowiedzi (na linię, na zapytanie o towary),
 - odpowiedzi inne niż prawidłowe oznaczać na czerwono.




 Paragony:
trinit	#911D
trline	naTowar A	vt0	pr9999999	#712E
trend	to9999999	#CC47

 Faktury:
trfvinit	#C8BA
trfvbuyer	naNazwaNabywcy#########################################_56	ninip45678901234567890	sc0	#6E80
trfvnumber	nbNumerFaktury#########################_40	sc0	#02AF
trline	naTowar C	vt0	pr9999999	#3147
trline	naTowar C	vt1	pr9999999	#3232
trline	naTowar C	vt0	pr9999999	#3147
trend	to9999999	#CC47

 Sprawdzanie bazy:
dbchkstart	ty1	#BDD1
dbchkplu	vt0	naTowar A	#8064
dbchkplu	vt0	naTowar B	#D537
dbchkend	#990F

dbchkstart	ty2	#E882
dbchkplu	vt0	naTowar A	#8064
dbchkend	#990F
  */

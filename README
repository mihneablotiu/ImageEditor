313CA - Bloțiu Mihnea-Andrei

Tema 3 - Image Editor

Pentru această temă am realizat inițial un sistem ce realizează citirea
de la tastatură a comenzilor. 
Acesta se bazează pe realizarea comenzii citite și așteptarea
introducerii unei noi comenzi până la întalnirea comenzii EXIT.

Pentru comanda de LOAD am realizat alocarea dinamică a matricei ce va
reține datele pozei și am citit datele din fișiere în mod diferit în
funcție de tipul de imagine primit. Astfel am setat și dimensiunile
inițiale are matricei.

Pentru selectarea unei regiuni, am ținut cont de posibila ordine
inversată a datelor primite printr-o interschimbare și am avut grijă ca
nu cumva dimensiunile primite să depășească dimensiunea matricei actuale.
Analog, în cadrul comenzii SELECT ALL, selectăm întreaga dimensiune a
matricei ținând cont de lungimea și înălțimea acesteia. 
Toate aceste operații au fost realizate folosind 4 variabile de tip întreg.

Pentru operația de ROTATE facem în primul rând distincția între ce fel de
poză este cea pe care dorim să o rotim și apoi ce fel de rotire se
realizează (totală sau a unui pătrat). 
Pentru fiecare dintre aceste cazuri am realizat o funcție de rotire
efectivă a matricei pentru 90 de grade pe care am aplicat-o de mai multe
ori în funcție de unghiul primit. 
Aceste funcțiee efective au fost introduse în operația totală ce
presupune copierea matricei inițiale, rotirea efectivă a copiei și
introducerea rezultatului în matricea inițială pe poziția
corespunzătoare, de unde a fost preluată.

În cadrul operației de CROP, ideea este similară cu precizarea că după
copierea selecției din matricea inițială, actualizăm și dimensiunile noii
matrice deoarece doar cu aceasta vom lucra pe viitor.

Pentru operațiile de GRAYSCALE și SEPIA verificăm în primul rând faptul
că aplicăm această operație doar pentru imaginile color. 
Apoi, folosind formulele precizate în enunț, calculăm noile valori pe
care le rotunjim la cel mai apropiat întreg.

Pentru salvare, realizăm două funcții separate în vederea salvării în
format text sau binar. 
Acestea sunt practic operațiile inverse pentru cea de LOAD.

În final, în cazul primirii comenzii de EXIT, nu mai este așteptată o
nouă comandă, realizăm eliberarea tuturor resurselor, iar programul se
încheie.

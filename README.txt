Romanian:

Am folosit o structura pentru fiecare task (la task-urile 2 si 3 am folosit
aceeasi structura). Astfel o structura de arbore este reprezentata prin
valoare nodului, nivelul sau alte elemente si de o lista de copii.

Citirea la fiecare task s-a realizat cu ajutorul unei matrice alocata dinamic
si care retinea inputul. Toata memoria a fost alocata dinamic, exceptand
stringurile cu ajutorul carora citeam linie cu linie si anumite elemente
de dimensiuni foarte mici pentru un strtok().

Taskul1:
    Pentru fiecare nod am retinut matricea in punctul respectiv. Arorele a 
    fost construit in mod recursiv pornind de la radacina si la fiecare
    apel trimiteam ca parametru Player-ul care urma sa faca mutarea.
    La fiecare mutare am verificat daca acea mutare va deveni nod frunza
    pentru a putea continua sau nu in adaugarea unui nou jucator. 
    La afisare am parcurs fiecare nod si cu ajutorul unei functii recursive,
    la fiecare apel am avut un numar de tab-uri, reprezentant nivelul pe care
    se afla nodul respectiv in arbore.

Taskul 2:
    Am construit arborele folosind o maniera iterativa. Astfel am parcurs
    matricea citita de jos in sus si am format o lista cu toate nodurile.
    Mergand in sus in arbore vedeam cati copii are fiecare nod si formam lista
    fiecaruia cat si ce valoare are tinand cont denivelul pe care se afla
    (Min sau Max). Astfel la final aveam o lista cu nodul root si cu Toata
    informatia necesara. Afisarea se realizeaza la fel ca la taskul 1.

Taskul 3:
    Initial am folosit algoritmul de la taskul 2 pentru a avea arborele format
    si apoi am aplicat algoritmul AlphaBeta pe acest arbore, mergand
    in subarborele stang si gasind coeficientii alpha si beta pentru fiecare
    nod. Astfel cu ajutorul coeficientilor am verificat daca in subarborele
    drept mai are rost sa merg sau il pot anula. Pe nivelele de tip Max am
    updatat alphaul, iar pe cele de tip Min Betaul. Astfel daca intr-un nod
    se ajungea la conditia beta <= alpha atunci nu mai avea rost sa caut mai 
    jos in arbore, deoarece oricum nodul parinte avea o solutie mult mai buna.
    Toata functia de reconstructie a arborelui este una recursiva.

Bonusul:
    Am citit informatia intr-o matrice si am parcurs-o de jos in sus.
    Am creeat o lista de noduri la fiecare nivel iar la final am avut
    doar un nod de lista care continea ROOT-ul arborelui.
    La acest task am impartit totul pe mai multe
    nivele. Daca de exemplu avem X jucatori, putem observa ca jucatorul Max
    se afla pe nivelul in care inaltimea la care se afla in acel moment
    are restul 1 la impartirea cu X (Level % X == 1). In rest sunt jucatori
    de tip Min care incearca sa minimizeze castigul.

Toata memoria a fost dealocata.

English:
Implemented minimax, aplha-beta and paranoid algorithms for gaming trees. 

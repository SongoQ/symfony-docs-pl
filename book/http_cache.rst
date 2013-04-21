.. highlight:: php
   :linenothreshold: 2

.. index::
   single: pamięć podręczna

Pamięć podręczna HTTP
=====================

Istotą bogatych aplikacji internetowych jest to, że są one dynamiczne. Bez względu
na to, jak skuteczna jest aplikacja, każde żądanie będzie zawsze powodować dodatkowy
narzut w obciążeniu serwera niż serwowanie statycznego pliku.

Dla większości aplikacji internetowych, to jest norma. Symfony2 jest błyskawiczne
i każde żądanie obsługiwane jest szybko, bez zbędnego obciążenia serwera,  chyba
że robi się w aplikacji „podnoszenie ciężarów”.

Gdy witryna się rozrasta, to ten narzut może stać się problemem. Przetwarzanie,
które zwykle jest wykonywane przy każdym żądaniu, powinno być wykonywane tylko
raz. To jest właśnie to, co ma osiągnąć buforowanie.

Buforowanie na barkach gigantów
-------------------------------

Najbardziej efektywnym sposobem na poprawę wydajności aplikacji jest buforowanie
całego wyjścia strony i ominięcie całkowite kodu aplikacji przy każdym kolejnym
żądaniu. Oczywiście nie jest to zawsze możliwe dla bardziej dynamicznych witryn
ale czy na pewno? W tym rozdziale zobaczysz jak działa system
buforowania Symfony2 i dlaczego jest to najlepsze z możliwych rozwiązań.

Pamięć podręczna Symfony2 jest inna ponieważ opiera się prostocie i mocy pamięci
podręcznej HTTP, tak jak to określono w :term:`specyfikacji HTTP<specyfikacja HTTP>`.
Symfony2 implementuje ten standard, ustalający podstawową komunikację w internecie,
zamiast wymyślać własną metodologię buforowania. Kiedy zrozumiesz podstawy walidacji
HTTP i modele wygasania buforowania, będziesz gotowy do opanowania systemu buforowania
Symfony2.

W celu wyjaśnienia systemu buforowania Symfony2, temat jest podzielony na cztery etapy:

#. :ref:`Pamięć podręczna bram bramy<gateway-caches>` lub odwrotne proxy
   (*ang. reverse proxy*), jest niezależną warstwą, która znajduje się na przed
   aplikacją. Pamięć podręczna bramy buforuje odpowiedzi, w miarę ich zwracania przez
   aplikację i odpowiada na żądania buforowanymi odpowiedziami, zanim dotrą do
   aplikacji. Symfony2 zapewnia własne odwrotnego proxy, ale można zastosować
   każdego inne rozwiązanie tego typu.

#. Nagłówki :ref:`pamięci podręcznej HTTP<http-cache-introduction>` są wykorzystywane
   do komunikacji z bramą buforującą i innymi buforami znajdującymi się pomiędzy
   aplikacją a klientem. Symfony2 zapewnia sensowne ustawienia domyślne i zaawansowany
   interfejs dla interakcji z nagłówkami pamięci podręcznej.

#. :ref:`Wygasanie i walidacja<http-expiration-validation>` HTTP są dwoma modelami
   używanymi w celu określenia czy buforowana zawartość jest świeża (może być użyta
   z pamięci podręcznej) lub nieaktualna (powinna być zregenerowana przez aplikację).

#. :ref:`Edge Side Includes <edge-side-includes>` (ESI) umożliwia aby bufor HTTP
   był używany niezależnie do buforowania fragmentów stron (nawet fragmentów
   zagnieżdżonych). Przy zastosowaniu ESI można nawet buforować całe strony na
   60 minut, ale wbudowany pasek boczny tylko przez 5 minut.

Ponieważ pamięć podręczna HTTP nie jest unikalna dla Symfony, to istnieje już wiele
artykułów na ten temat. Jeśli jesteś nowicjuszem w buforowaniu HTTP, to bardzo polecana
jest lektura artykułu Ryana Tomayko `Things Caches Do`_ is. Innym dogłębnym źródłem
wiedzy jest `Cache Tutorial`_ Marka Nottinghama.

.. index::
   single: pamięć podręczna; proxy
   single: pamięć podręczna; odwrotne proxy
   single: pamięć podręczna; brama

.. _gateway-caches:

Buforowanie z pamięcią podręczną bramy
--------------------------------------

Podczas buforowania HTTP, pamięć podręczna jest oddzielona całkowicie od aplikacji
i znajduje się pomiędzy aplikacją a klientem wykonującym żądanie.

Zadaniem pamięci podręcznej jest przyjmowanie żądań od klienta i przekazywanie ich
do aplikacji. Pamięć podręczna również otrzymuje odpowiedzi z aplikacji i wysyła
je do klienta. Pamięć podręczna jest "pośrednikiem" komunikacji żądanie-odpowiedź,
pośrednicząc pomiędzy klientem a aplikacją.

Po drodze pamięć podręczna przechowuje każdą odpowiedź, która uważana jest za
"buforowalną" (patrz :ref:`http-cache-introduction`). Jeśli żądany jest ponownie
ten sam zasób, to pamięć podręczna przesyła od razu klientowi buforowaną odpowiedź,
ignorując całkowicie aplikację.

Ten typ pamięci podręcznej jest nazywany **pamięcią bramy HTTP**
 (*ang. HTTP gateway cache*) i istnieje wiele rozwiązań tego typu, takich jak
 `Varnish`_, `Squid w trybie odwrotnego proxy`_, czy odwrotne proxy Symfony2.

.. index::
   single: pamięć podręczna; typy

Typy pamięci podręcznej
~~~~~~~~~~~~~~~~~~~~~~~

Pamięć podręczna bramy, to nie jedyny typ pamięci podręcznej. Faktycznie nagłówki
HTTP, które są wysyłane przez aplikację są wykorzystywane i interpretowane przez
trzy różne typy pamięci podręcznej:

* **Pamięć podręczna przeglądarki** (*ang. browser cache*): każda przeglądarka posiada
  własną lokalną pamięć podręczną, która jest głównie wykorzystywana do obsługi
  "wstecznego" przeglądania stron lub buforowania obrazów i innych aktywów. Pamięć
  podręczna przeglądarki jest pamięcią prywatną, buforującą zasoby  nie udostępniane
  komu innemu, niż lokalny użytkownik;

* **Pamięć podręczna serwera pośredniczącego, lub pamięć podręczna serwera proxy**
  (*ang. proxy cache*): pamięć serwera pośredniczącego jest pamięcią działającą
  na wydzielonym serwerze lub w postaci samodzielnego oprogramowania i jest
  współdzielona przez wielu użytkowników. Jest zwykle instalowana w dużych sieciach
  korporacyjnych oraz u dostawców internetu (ISP) w celu redukcji czasu oczekiwania
  i obciążenia sieci;

* **Pamieć podręczna bramy** (*ang. gateway cache*):
  podobna jest do pamięci podręcznej serwera pośredniczącego, jest też pamięcią
  współdzieloną ale działa po stronie serwera internetowego. Zainstalowanie jej
  przez administratora czyni witryny bardziej skalowalne, niezawodne i wydajne.

.. tip::

    Pamięć podręczna bramy jest też nazywana odwrotnym proxy (*ang. reverse proxy*)
    pamięcią podręczną zastępczą (*ang. surrogate cache*) lub nawet akceleratorem
    HTTP (*ang. HTTP accelerator*). W niniejszym podręczniku będziemy używać
    zamiennie nazw: *pamięć podręczna bramy* lub *odwrotne proxy*. 
        
.. note::

    Znaczenia pamięci *prywatnej* i *współdzielonej* staną się bardziej oczywiste,
    po omówieniu buforowania odpowiedzi zawierających treści specyficzne 
    dla jednego użytkownika (np. informacje o koncie).

Każda odpowiedź z aplikacji będzie prawdopodobnie przechodzić przez jeden lub dwa
wymienione wyżej, w pierwszej kolejności, typy pamięci. Te pamięci są poza Twoją
kontrolą, ale działają zgodnie z ustawieniem pamięci podręcznej HTTP znajdującym
się w nagłówku odpowiedzi.

.. index::
   single: pamięć podręczna; odwrotne proxy Symfony2

.. _`symfony-gateway-cache`:

Odwrotne proxy Symfony2
~~~~~~~~~~~~~~~~~~~~~~~

Symfony2 dostarczane jest z odwrotnym proxy (pamięcią podręczną bramy) napisanym w PHP.
Po włączeniu tej funkcjonalności buforowalne odpowiedzi z aplikacji
będą od razu buforowane. Zainstalowanie odwrotnego proxy jest również proste.
Każda nowa aplikacja Symfony2 dostarczana jest ze wstępnie skonfigurowanym jądrem
buforowania (``AppCache``), które opakowuje domyślny (``AppKernel``). Jądro buforowania,
to odwrotne proxy.

Aby włączyć buforowanie, trzeba poprawić kod kontrolera wejścia, tak aby używał
jądro buforowania::

    // web/app.php
    require_once __DIR__.'/../app/bootstrap.php.cache';
    require_once __DIR__.'/../app/AppKernel.php';
    require_once __DIR__.'/../app/AppCache.php';

    use Symfony\Component\HttpFoundation\Request;

    $kernel = new AppKernel('prod', false);
    $kernel->loadClassCache();
    // opakowanie domyślnego AppKernel w AppCache
    $kernel = new AppCache($kernel);
    $request = Request::createFromGlobals();
    $response = $kernel->handle($request);
    $response->send();
    $kernel->terminate($request, $response);

Jądro buforowania będzie działać natychmiast jako odwrotne proxy – będzie buforować
odpowiedzi z aplikacji i zwracać je klientowi.

.. tip::

    Jądro buforowania ma specjalną metodę ``getLog()``, która zwraca łańcuch
    reprezentujący, to co zdarzyło się w warstwie buforowania. W środowisku
    programistycznym można wykorzystać ją do debugowania i walidacji strategii
    buforowania::

        error_log($kernel->getLog());

Obiekt ``AppCache`` ma sensowną domyślną konfigurację, ale można ją dostosować
poprzez zestaw opcji, jaki można ustawić przez nadpisanie metody
:method:`Symfony\\Bundle\\FrameworkBundle\\HttpCache\\HttpCache::getOptions`::

    // app/AppCache.php
    use Symfony\Bundle\FrameworkBundle\HttpCache\HttpCache;

    class AppCache extends HttpCache
    {
        protected function getOptions()
        {
            return array(
                'debug'                  => false,
                'default_ttl'            => 0,
                'private_headers'        => array('Authorization', 'Cookie'),
                'allow_reload'           => false,
                'allow_revalidate'       => false,
                'stale_while_revalidate' => 2,
                'stale_if_error'         => 60,
            );
        }
    }

.. tip::

    Jeśli nie nadpisze się ``getOptions()``, to opcja ``debug`` będzie ustawiona
    na automatyczne debugowanie wartości opakowania ``AppKernel``.

Oto lista głównych opcji:

* ``default_ttl``: okres w sekundach, przez który buforowany wpis jest uznawany
  jako świeży, gdy w odpowiedzi nie ma żadnej informacji odświeżającej. Wartość tą
  (domyślnie: ``0``) nadpisuje jawne ustawienie nagłówków ``Cache-Control`` lub ``Expires``;

* ``private_headers``: ustawienie nagłówków odpowiedzi, które wyzwalają "prywatne"
  zachowanie ``Cache-Control``w odpowiedziach jawnie nie precyzujących czy odpowiedź
  jest ``public`` czy też ``private`` poprzez dyrektywę ``Cache-Control`` (domyślnie:
  ``Authorization`` i ``Cookie``);

* ``allow_reload``: określa, czy klient może wymuszać ponowne ładowanie pamięci
  podręcznej przez dołączenie  w odpowiedzi "nie buforowej" dyrektywy ``Cache-Control``.
  Aby osiągnąć zgodności z RFC 2616, nalezy ustawić tą opcje na ``true``
  (domyślnie: ``false``);

* ``allow_revalidate``: określa, czy klient może wymusić odświeżenie danych 
  zawartości pamięci podręcznej przez dołączenie do żądania dyrektywy ``Cache-Control``
  "max-age=0". W celu osiągnięcia zgodności z RFC 2616 trzeba ustawić tą opcję na
  ``true`` (default: false);

* ``stale_while_revalidate``: określa domyślny okres w sekundach podczas którego
  pamięć podręczna może natychmiast zwrócić starą odpowiedź gdy odnawianie
  pamięci podręcznej realizowane jest w tle (domyślnie: ``2``). Ustawienie to jest
  nadpisywane przez opcję ``stale-while-revalidate`` rozszerzenia HTTP
  ``Cache-Control`` (zobacz RFC 5861);

* ``stale_if_error``: określa domyślny okres w sekundach podczas którego pamięć
  podręczna może serwować nie odświeżoną odpowiedź gdy wystąpił błąd (domyślnie:
  ``60``). Ustawienie to nadpisywane jest przez opcję ``stale-if-error`` rozszerzenia
  HTTP ``Cache-Control`` (zobacz RFC 5861).

Jeśli ``debug`` jest ustawione na ``true``, Symfony2 automatycznie dodaje do odpowiedzi
nagłówek ``X-Symfony-Cache``, który zawiera użyteczne informacje o odsłonach pamięci
podręcznej i niebezpieczeństwach.

.. sidebar:: Zmiana jednego odwrotnego proxy na inne

    Odwrotne proxy Symfony2 jest doskonałym narzędziem do wykorzystania przy tworzeniu
    witryny internetowej lub podczas jej wdrażania na współdzielonym hoście, gdzie
    nie można zainstalować niczego innego niż kod PHP. Lecz co jest napisane w PHP,
    to nie może być takie szybkie jak proxy napisane w C. Dlatego zaleca się użycie
    serwerów Varnish lub Squid na swoim serwerze produkcyjnym, jeśli jest to możliwe.
    Dobrą wiadomością jest to, że przejście z jednego serwera proxy na inny jest
    łatwe i przejrzyste, jako że nie jest konieczna zmiana kodu aplikacji.
    Zacznij najpierw z odwrotnym proxy Symfony2 i później, jak wzrośnie ruch na
    witrynie, to zamień odwrotne proxy na Varnish.

    Więcej informacji o użyciu Varnish z Symfony2 znajdziesz w artykule
    :doc:`Jak uzywać Varnish </cookbook/cache/varnish>`.

.. note::

    Wydajność odwrotnego proxy Symfony2 jest niezależne od złożoności aplikacji.
    Dzieje się tak, bo jadro aplikacji jest uruchamiane tylko wówczas, gdy żądanie
    musi być do niego skierowane.

.. index::
   single: pamięć podręczna; HTTP

.. _http-cache-introduction:

Wstęp do buforowania HTTP
-------------------------

Aby skorzystać z dostępnej warstw pamięci podręcznej, aplikacja musi być informowana,
które odpowiedzi są buforowalne oraz z zasadami, które regulują, kiedy (jak) pamięć
podręczna powinna się zdezaktualizować. Odbywa się to poprzez ustawienie w odpowiedzi
nagłówków buforowania HTTP.

.. tip::

    Należy pamiętać, że "HTTP" jest niczym innym jak językiem (prostym językiem
    tekstowym), który klienci internetowi (np. przeglądarki) i serwery internetowe
    używają do wzajemnej komunikacji. Buforowanie HTTP jest częścią tego języka,
    która umożliwia klientom i serwerom wymianę informacji związanych z buforowaniem.

HTTP określa cztery nagłówki buforowania w odpowiedzi. Oto one:

* ``Cache-Control``
* ``Expires``
* ``ETag``
* ``Last-Modified``

Najważniejszym i najczęściej używanym nagłówkiem jest ``Cache-Control``,
który w rzeczywistości jest zbiorem różnych informacji o pamięci podręcznej.

.. note::

    Każdy z nagłówków jest szczegółowo omówiony w rozdziale
    :ref:`http-expiration-validation`.

.. index::
   single: pamięć podręczna; nagłówek
   single: nagłówki HTTP; Cache-Control

Nagłówek Cache-Control
~~~~~~~~~~~~~~~~~~~~~~

Nagłówek ``Cache-Control`` jest wyjątkowy, ponieważ zawiera nie jedną ale wiele
porcji informacji o buforowaniu odpowiedzi. Każda z tych porcji jest oddzielona
przecinkiem:

     Cache-Control: private, max-age=0, must-revalidate

     Cache-Control: max-age=3600, must-revalidate

Symfony dostarcza abstracji nagłówka ``Cache-Control`` w celu ułatwienia jego tworzenia::

    // ...

    use Symfony\Component\HttpFoundation\Response;

    $response = new Response();

    // zaznaczenie odpowiedzi jako publicznej lub prywatnej
    $response->setPublic();
    $response->setPrivate();

    // ustawienie max age jako prywatje lub współdzielonej 
    $response->setMaxAge(600);
    $response->setSharedMaxAge(600);

    // ustawienie własnej dyrektywy Cache-Control
    $response->headers->addCacheControlDirective('must-revalidate', true);


.. _public_vs_private_respnses:

Odpowiedzi publiczne vs prywatne
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Zarówno pamięć podręczna bramy jak i pamięć serwera pośredniczącego są uważane za
pamięci "współdzielone", ponieważ zawartość buforowana jest udostępniana więcej niż
jednemu użytkownikowi. Jeśli odpowiedzi specyficzna dla jednego użytkownika byłyby
kiedyś błędnie zapisane we współdzielonej pamięci, to  mogłyby być później zwrócone
innym użytkownikom. Wyobraź sobie, ze informacja o Twoim koncie jest buforowana
i później przesyłana każdemu użytkownikowi, który by zażądał tej informacji. Zgroza!

Dlatego każda odpowiedź jest ustawiana jako publiczna albo prywatna, co umożliwia
obsłużenie takiej sytuacji:

* *public*: wskazuje, że odpowiedź może być buforowana zarówno w pamięci prywatnej
  jak i współdzielonej;

* *private*: wskazuje, że wszystkie lub część komunikatów odpowiedzi są przeznaczone
  dla pojedynczego użytkownika  i nie mogą być buforowane w pamięci współdzielonej.

Symfony konserwatywnie traktuje w sposób domyślny każdą odpowiedź jako prywatną.
Aby skorzystać z wielodostępnych buforów (jak odwrotne proxy Symfony2), odpowiedź
musi jawnie zostać określona jako publiczna.

.. index::
   single: pamięć podręczna; bezpieczne metody

Bezpieczne metody
~~~~~~~~~~~~~~~~~

Buforowanie HTTP działa tylko dla "bezpiecznych" metod HTTP (takich jak GET i HEAD).
Za bezpieczne uważa się te metody HTTP, które nigdy nie zmieniają stanu aplikacji na
serwerze podczas serwowania odpowiedzi (można oczywiście zwrócić informacje dziennika,
datę buforowania itp.). Ma to dwie bardzo pozytywne konsekwencje:

* Nigdy nie powinno się zmieniać stanu aplikacji podczas przesyłania odpowiedzi
  GET lub HEAD. Nawet jeśli nie używa się pamięci bramy, występowanie po drodze
  pamięci serwera pośredniczącego (proxy) oznacza, że każda odpowiedź GET lub HEAD
  może ale nie musi dotrzeć faktycznie do serwera aplikacji;

* Nie należy oczekiwać metod PUT, POST lub DELETE w odpowiedziach buforowanych.
  Metody te przeznaczone są do zmieniania stanu aplikacji (np. usunięcia wpisu na
  blogu). Buforowanie ich uniemożliwiłoby niektórym żądaniom dotarcie do aplikacji
  i zmianę jej stanu.

Zasady buforowania i wartości domyślne
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

HTTP 1.1 umożliwia domyślnie buforowanie wszystkiego, chyba że istnieje nagłówek
``Cache-Control``. W praktyce, większość buforów nie robi nic, gdy żądania mają
pliki cookie, nagłówek autoryzacyjny, używają metod niezaliczanymi do bezpiecznych
(tj. PUT, POST, DELETE) lub gdy odpowiedzi mają kod statusu przekierowania.

Symfony2 automatycznie ustawia sensowny i konserwatywny nagłówek ``Cache-Control``,
gdy żaden nie jest ustawiony przez programistę, wg następujących zasad:

* Jeśli nie jest określony żaden nagłówek buforowania (``Cache-Control``, ``Expires``,
  ``Etag`` lub ``Last-Modified``), ustawiany jest nagłówek ``Cache-Control``
  z wartością ``no-cache``, co oznacza, że odpowiedź nie będzie buforowana;

* Jeśli ``Cache-Control`` jest puste (ale obecny jest jeden z pozostałych nagłówków
  buforowania), to jego wartość jest ustawiana na ``private, must-revalidate``;

* Lecz jeśli jest ustawiona co najmniej jedna dyrektywa ``Cache-Control`` i nie
  zostały jawnie dodane dyrektywy 'public' lub ``private``, to Symfony2 doda
  automatycznie dyrektywę ``private`` (z wyjątkiem, gdy ustawione jest ``s-maxage``).


.. index::
   single: pamięć podręczna; wygasanie HTTP

.. _http-expiration-validation:

Wygasanie i walidacja HTTP
--------------------------

Specyfikacja HTTP definiuje dwa modele buforowania:

* W `modelu wygasania`_, określa się jak długo odpowiedź uważana jest za "świeżą"
  (*ang. fresh*) przez dołączenie nagłówka ``Cache-Control`` i ewentualnie ``Expires``.
  Pamięci rozumiejące wygasanie nie będą wykonywać ponownie takiego samego żądania
  dopóki buforowana wersja zasobu nie przekroczy czas wygasania i stanie się
  "przestarzała" (*ang. stale);

* Gdy strony są naprawdę dynamiczne (czyli często następują zmiany w reprezentacji),
  to często niezbędny jest `model walidacyjny`_ . W tym modelu odpowiedź jest buforowana,
  ale pamięć zwraca się z zapytaniem do serwera przy każdym żądaniu, czy buforowana
  odpowiedź jest nadal aktualna. Aplikacja używa unikalny identyfikator odpowiedzi
  (nagłówek ``Etag``) i ewentualnie sygnaturę czasu (nagłówek ``Last-Modified``)
  do sprawdzenia czy na stronie dokonano zmian od czasu ostatniego buforowania.

Celem obydwu modeli jest spowodowanie, aby ta sama odpowiedź nie była nigdy generowana
dwa razy, przez wymuszenie na pamięci podręcznej zapisywania i zwracania "świeżych"
odpowiedzi.

.. sidebar:: Czytanie specyfikacji HTTP

    Specyfikacja HTTP definiuje prosty ale potężny język, w którym klienci i serwery
    mogą się komunikować. Dla programisty aplikacji internetowych, model
    żądanie-odpowiedź jest dominujący w jego pracy. Niestety, oryginalny dokument
    specyfikacji (`RFC 2616`_ ) może być trudny w czytaniu.

    Podjęta jest próba przerobienia oryginalnego dokumentu „RFC 2616” na bardziej
    przystępną wersję (`HTTP Bis`_). Nie opisuje ona nowego standardu HTTP, ale
    przede wszystkim wyjaśnia oryginalną specyfikację HTTP. Poprawiła się też
    organizacja dokumentu. Opis podzielony został na siedem części. Wszystko co
    związane jest z buforowaniem HTTP można znaleźć w dwóch dedykowanych częściach
    (`P4 - Conditional Requests`_ i `P6 - Caching: Browser and intermediary caches`_).

    Gorąco zachęcamy Ciebie, jako programistę aplikacji internetowych, do zapoznania
    się z tym dokumentem specyfikacji. Jego przejrzystość i kompleksowość informacji,
    nawet dziesięć lat po stworzeniu, są imponujące. Nie zniechęcaj się jego
    wyglądem – zawarta tam treść jest znacznie piękniejsza niż okładka.


Wygasanie
~~~~~~~~~
Model wygasania jest bardziej wydajny i prostszy z dwóch, poprzednio przedstawionych,
modeli buforowania i powinien być stosowany w miarę możliwości. Gdy odpowiedź jest
buforowana z wygasaniem, pamięć będzie przechowywać odpowiedź i zwraca ją bezpośrednio
bez przekazywania żądania do aplikacji do momentu jego wygaśnięcia.

Model wygasania może zastosować używając jednego z dwóch nagłówków HTTP, niemal
identycznych: ``Expires`` lub ``Cache-Control``.


.. index::
   single: pamięć podręczna; nagłówek Expires
   single: nagłówki HTTP; Expires

Wygasanie - nagłówek ``Expires``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Zgodnie ze specyfikacją HTTP, "pole nagłówka ``Expires`` podaje datę/czas po którym
odpowiedź jest uważana za przestarzała". Nagłówek ``Expires`` może zostać ustawiony
poprzez metodę ``setExpires()`` obiektu ``Response``. Wymaga to jako argumentu
instancji ``DateTime``::

    $date = new DateTime();
    $date->modify('+600 seconds');

    $response->setExpires($date);

W rezultacie nagłówek HTTP bedzie wyglądać tak:

.. code-block:: text

    Expires: Thu, 01 Mar 2011 16:00:00 GMT

.. note::

    Metoda ``setExpires()`` przekształca automatycznie datę na datę strefy czasowej
    GMT, tak jak to jest wymagane w specyfikacji.

Należy zaznaczyć, że w wersjach HTTP przed wersją 1.1 oryginalny serwer nie był
zobowiązany do wysyłania nagłówka ``Date``. W efekcie pamięć podręczna (np. przeglądarki)
musiała polegać na swoim zegarze wewnętrznym przy ocenie nagłówka ``Expires`` wykonując
przeliczenie czasu życia podatne na przesunięcie czasowe. Innym ograniczeniem nagłówka
``Expires`` jest ustalenie specyfikacji stanowiące, że "serwery HTTP/1.1 nie powinny
wysyłać dat ``Expires`` przekraczających roczny okres."

.. index::
   single: pamięć podręczna; nagłówek Cache-Control
   single: nagłówki HTTP; Cache-Control

Wygasanie - nagłówek ``Cache-Control``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Ze względu na ograniczenia nagłówka ``Expires``, w większości przepadków powinno
się zastosować zamiast niego nagłówek ``Cache-Control``. Przypomnijmy, że nagłówek
``Cache-Control`` jest używany do określenia wielu różnych dyrektyw buforowania.
Dla wygasania istnieją  dwie dyrektywy: ``max-age`` i ``s-maxage``. Pierwsza z nich
jest używana przez wszystkie rodzaje pamięci, natomiast druga jest brana pod uwagę
tylko przez pamięci współdzielone::

    // Ustawienie ilości sekund po upływie których odpowiedź
    // nie powinna być uważana za świeżą
    $response->setMaxAge(600);

    // To samo jak wyżej ale tylko dla pamięci współdzielonej
    $response->setSharedMaxAge(600);

Nagłówek ``Cache-Control`` będzie miał następujący format (może to mieć kilka
dodatkowych dyrektyw):

.. code-block:: text

    Cache-Control: max-age=600, s-maxage=600

.. index::
   single: pamięć podręczna; walidacja

Walidacja
~~~~~~~~~

Model wygasania zawodzi, gdy zasób musi być aktualizowany jak tylko zostaną dokonane
zmiany podstawowych danych. W modelu wygasania aplikacja nie zostanie poproszona
o zwrócenie zaktualizowanego zasobu dopóki buforowany zasób nie stanie się przestarzały.

Model walidacyjny rozwiązuje ten problem. W modelu tym pamięć kontynuuje przechowywanie
odpowiedzi. Różnica jest taka, że dla każdego żądania pamięć pyta aplikację czy
buforowany zasób jest jeszcze aktualny. Jeśli pamięć jest jeszcze aktualna, to aplikacja
powinna zwrócić kod statusu 304 i żadnej zawartości. Powiadamia to pamięć, aby zwróciła
użytkownikowi buforowany zasób.

W modelu tym oszczędza się przede wszystkim na przepustowości łącza, ponieważ
reprezentacja danych nie jest dwa razy wysyłana do tego samego klienta (zamiast
tego wysyłany jest kod statusu 304). Lecz jeśli projektuje się aplikację starannie,
to ma się możliwość uzyskania minimum danych koniecznych do wysłania odpowiedzi 304
a nawet zaoszczędzenia czasu CPU (zobacz poniżej na przykładową implementację).

.. tip::

    Kod statusu 304 oznacza "Nie zmieniono". Jest to ważne, ponieważ ten kod statusu
    nie zawiera aktualnej treści, która ma być odesłana w odpowiedzi. Zamiast treści
    jest po prostu krótka wskazówka powiadamiająca pamięć aby wysłała użytkownikowi
    buforowaną wersję strony.

Podobnie jak w modelu wygasania istnieją dwa różne nagłówki HTTP, które można
zastosować w modelu walidacyjnym: ``ETag`` i ``Last-Modified``.

.. index::
   single: pamięć podręczna; nagłówek Etag
   single: nagłówki HTTP; Etag

Walidacja - nagłówek ``ETag``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Nagłówek ``ETag`` jest nagłówkiem łańcuchowym (nazywanym "entity-tag"), który
jednoznacznie identyfikuje  reprezentację zasobu docelowego. Jest on całkowicie
generowany i ustawiany przez aplikację, tak aby można informować, na przykład,
czy przechowywany w pamięci podręcznej zasób ``/about``został w międzyczasie
zaktualizowany i musi być zwrócony przez aplikację zamiast buforowanej wersji.
Nagłówek ``ETag`` jest jak odcisk palca i służy do szybkiego  porównania, czy dwie
różne wersje zasobu są równoważne. Podobnie jak odcisk palca, każdy nagłówek ``ETag``
musi być unikalny dla poszczególnych reprezentacji tego samego zasobu.

Aby zobaczyć prostą implementacje generującą nagłówek ETag jako md5 zawartości
zasobu, wykonajmy to::

    public function indexAction()
    {
        $response = $this->render('MyBundle:Main:index.html.twig');
        $response->setETag(md5($response->getContent()));
        $response->setPublic(); // make sure the response is public/cacheable
        $response->isNotModified($this->getRequest());

        return $response;
    }

Metoda :method:`Symfony\\Component\\HttpFoundation\\Response::isNotModified`
porównuje nagłówek ``ETag`` ustawiony w obiekcie ``Request`` z nagłówkiem odpowiedzi
ustawionym w obiekcie ``Response``. Jeśli są one zgodne, to metoda ta automatycznie
ustawia ``Response`` na kod statusu 304.

Algorytm ten jest dość prosty i bardzo ogólny, Ale trzeba utworzyć cały obiekt
``Response`` przed obliczeniem ETag, co jest nieoptymalne. Innymi słowami, pozwala
to na oszczędność przepustowości ale nie czasu użycia CPU.

W rozdziale :ref:`optimizing-cache-validation` zobaczysz jak walidacja może zostać
wykorzystana w bardziej inteligentny sposób do ustalenia aktualności pamięci podręcznej
bez nadmiernego wysiłku.

.. tip::

    Symfony2 obsługuje również słabe nagłówki Etag, gdy przekaże się wartość ``true``
    jako drugi argument metody :method:`Symfony\\Component\\HttpFoundation\\Response::setETag`.

.. index::
   single: pamięć podręczna; nagłówek Last-Modified
   single: nagłówki HTTP; Last-Modified

Walidacja - nagłówek ``Last-Modified``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Nagłówek ``Last-Modified`` jest drugą forma walidacji. Zgodnie ze specyfikacją HTTP:
"Pole nagłówka ``Last-Modified`` wskazuje datę i czas po jakim oryginalny serwer
uważa reprezentację ostatniej modyfikacji zasobu za przestarzałą". Innymi słowami,
aplikacja określa czy w pamięci podręcznej buforowana zawartość ma zostać zaktualizowana
opierając się na informacji o terminie ważności tej zawartości.

Na przykład, można wykorzystać dla wszystkich obiektów datę ostatniej modyfikacji 
niezbędną do obliczenia daty ważności reprezentacji zasobu jako wartość nagłówka
``Last-Modified``::

    public function showAction($articleSlug)
    {
        // ...

        $articleDate = new \DateTime($article->getUpdatedAt());
        $authorDate = new \DateTime($author->getUpdatedAt());

        $date = $authorDate > $articleDate ? $authorDate : $articleDate;

        $response->setLastModified($date);
        // Ustawienie odpowiedzi jako publicznej.
        // W przeciwnym razie zostanie ona domyślnie ustawiona jako prywatna
        $response->setPublic();

        if ($response->isNotModified($this->getRequest())) {
            return $response;
        }

        // ... wykonanie jeszcze czegoś aby wypełnić odpowiedź treścią

        return $response;
    }

Metoda :method:`Symfony\\Component\\HttpFoundation\\Response::isNotModified`
porównuje nagłówek ``If-Modified-Since`` wysłany w żądaniu z nagłówkiem ``Last-Modified``
w odpowiedzi. Jeśli są równoważne, to obiekt ``Response`` wyśle kod statusu 304.

.. note::

    Nagłówek żądania ``If-Modified-Since`` jest równoważny nagłówkowi ``Last-Modified``
    ostatnio przesłanej odpowiedzi do klienta dla danego zasobu. W ten sposób klient
    i serwer komunikują się wzajemnie i decydują, czy buforowany zasób został w międzyczasie
    zaktualizowany.

.. index::
   single: pamięć podręczna; warunkowe pobieranie odpowiedzi
   single: HTTP; 304

.. _optimizing-cache-validation:

Optymalizowanie kodu poprzez walidację
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Głównym celem każdej strategii buforowania jest złagodzenie obciążenia aplikacji.
Mówiąc inaczej, im mniej się zmusza aplikację do zwracania odpowiedzi 304, tym lepiej.
Metoda ``Response::isNotModified()`` wykonuje to dokładnie, przez udostępnienie
prostego i wydajnego wzorca::

    use Symfony\Component\HttpFoundation\Response;

    public function showAction($articleSlug)
    {
        // Pobranie minimum informacji do obliczenia
        // ETag lub wartości Last-Modified
        // (w oparciu o Request, dane są pobierane
        // z bazy danych lub na przykład z listy par klucz-wartość
        $article = ...;

        // utworzenie Response z ETag  i ewentualnie nagłówka Last-Modified
        $response = new Response();
        $response->setETag($article->computeETag());
        $response->setLastModified($article->getPublishedAt());

        // Ustawienie odpowiedzi jako publicznej. W przeciwnym razie będzie ona prywatna
        $response->setPublic();

        // Sprawdzenie czy Response zostało zmodyfikowane dla danego Request
        if ($response->isNotModified($this->getRequest())) {
            // natychmiastowe zwrócenie Response 304
            return $response;
        } else {
            // wykonanie tutaj jeszcze czegoś - jak np. pobieranie więcej danych
            $comments = ...;

            // lub renderowanie szablonu z $response który już się rozpoczął
            return $this->render(
                'MyBundle:MyController:article.html.twig',
                array('article' => $article, 'comments' => $comments),
                $response
            );
        }
    }

Gdy ``Response`` nie jest zmodyfikowane, to metoda ``isNotModified()`` autoamtycznie
ustawia kod statusu odpowiedzi na ``304``, usuwając treść i jakieś nagłówki, które
nie muszą być obecne w odpowiedzi ``304`` (zobacz
:method:`Symfony\\Component\\HttpFoundation\\Response::setNotModified`).

.. index::
   single: pamięć podręczna; nagłówek Vary
   single: nagłówki HTTP; Vary

Różnicowanie odpowiedzi
~~~~~~~~~~~~~~~~~~~~~~~

Dotąd zakładaliśmy, że każdy adres URI ma dokładnie jedną reprezentację docelowego
zasobu. Domyślnie buforowanie HTTP jest realizowane z wykorzystaniem   adresu URI
jako klucza buforu. Jeśli dwie osoby żądają tego samego adresu URI buforowanego
zasobu, druga osoba otrzyma wersję buforowaną.

Czasem to nie wystarcza i różne wersje tego samego adresu URI muszą zostać buforowane
na podstawie jednej lub większej ilości wartości nagłówkowych żądania. Na przykład,
jeśli kompresuje się strony, gdy klient ją obsługuje, każdy podany adres URI ma dwie
reprezentacje: jedna dla klienta obsługujacego kompresję i drugą gdy tak nie jest.
Oznaczenie jest realizowane przez wartość nagłówka żądania ``Accept-Encoding``.

W takim przypadku potrzeba bufora do przechowywania zarówno wersji odpowiedzi
skompresowanej jak i nieskompresowanej dla określonego adresu URI i zwracania
ich w oparciu o wartość ``Accept-Encoding``. Jest to realizowane przez użycie
nagłówka odpowiedzi ``Vary``, który jest listą rozdzielanych przecinkiem różnych
nagłówków, których wartości wyzwalają określoną dla siebie reprezentację żądanego
zasobu:

.. code-block:: text

    Vary: Accept-Encoding, User-Agent

.. tip::

    Ten szczególny nagłówek ``Vary`` buforuje różne wersje tego samego zasobu
    w oparciu o adres URI oraz wartość nagłówka żądania ``Accept-Encoding``
    i ``User-Agent``.

Obiekt ``Response`` oferuje przejrzysty interfejs dla zarządania nagłówkiem
``Vary``::

    // ustawienie jednego nagłówka Vary
    $response->setVary('Accept-Encoding');

    // ustawienie wielu nagłówków Vary
    $response->setVary(array('Accept-Encoding', 'User-Agent'));

Metoda ``setVary()`` pobiera nazwę nagłówka lub tablicę nazw nagłówków od których
zależą odpowiedzi.

Wygasanie i walidacja
~~~~~~~~~~~~~~~~~~~~~

Można oczywiście użyć w tym samym obiekcie ``Response`` obu metod walidacji i wygasania.
Ponieważ w wielu przypadkach wygasanie jest lepsze niż walidacja, to można mieć duży
pożytek ze stosowania obu metod. Innymi słowami, wykorzystując obie metody, można
polecić pamięci podręcznej obsługę buforowanej zawartości podczas ponownego sprawdzenia
(wygasanie) w pewnym przedziale aby zweryfikowała czy zawartość jest jeszcze świeża.

.. index::
    pair: pamięć podręczna; konfiguracja

Więcej metod Response
~~~~~~~~~~~~~~~~~~~~~

Klasa Response zapewnia wiele więcej metod odnoszących się do pamięci podręcznej.
Oto najbardziej przydatne z nich::

    // Oznacza przestarzała odpowiedź
    $response->expire();

    // Wymusza zwrócenie odpowiedzi 304 bez zawartości
    $response->setNotModified();

Dodatkowo większość nagłówków dotyczących buforowania może być ustawiana przez
pojedynczą metodę :method:`Symfony\\Component\\HttpFoundation\\Response::setCache`::

    // Konfiguruje ustawienia pamięci podręcznej w jednym wywołaniu
    $response->setCache(array(
        'etag'          => $etag,
        'last_modified' => $date,
        'max_age'       => 10,
        's_maxage'      => 10,
        'public'        => true,
        // 'private'    => true,
    ));


.. index::

  single: pamięć podręczna ESI
  single: ESI

.. _edge-side-includes:

Wykorzystywanie Edge Side Includes
----------------------------------

Pamięci podręczne bramy są świetnym sposobem na ulepszenie swojej aplikacji.
Lecz mają pewne
ograniczenie: mogą tylko buforować całe strony. Jeśli ma się bardziej dynamiczne
strony lub strona ma więcej dynamicznych części, to pech. Na szczęście Symfony2
oferuje rozwiązanie dla takich przypadków, oparte na technice nazywanej `ESI`_,
lub Edge Side Includes. Specyfikację tej techniki napisana została prawie 10
lat temu przez Akamaï. Technika ta umożliwia, aby określone części strony miały
inną strategię buforowania niż cała strona.

Opisane w specyfikacji ESI znaczniki można umieścić na stronie w celu komunikacji
z pamięcią podręczną bramy. W Symfony2 wykorzystany jest tylko jeden znacznik,
``include``, ponieważ jest to jedyny przydatny znacznik poza kontekstem Akamaï:

.. code-block:: html
   :linenos:

    <!DOCTYPE html>
    <html>
        <body>
            <!-- ... some content -->

            <!-- Embed the content of another page here -->
            <esi:include src="http://..." />

            <!-- ... more content -->
        </body>
    </html>

.. note::

    Proszę zwrócić uwagę, że każdy znacznik ESI ma w pełni kwalifikowany adres URL.
    Znacznik ESI reprezentuje fragment strony, który można pobrać poprzez dany
    adres URL.

Podczas obsługi żądania, pamięć podręczna bramy pobiera całą stronę ze swojego bufora
lub żąda jej z zaplecza aplikacji. Jeśli odpowiedź zawiera jeden lub więcej znaczników
ESI, są one przetwarzane w ten sam sposób. Innymi słowami, pamięć podręczna bramy
pobiera dołączane fragmenty strony ze swojego bufora albo żąda ponownie tego
fragmentu strony z zaplecza aplikacji. Kiedy wszystkie znaczniki ESI zostają rozwiązane,
pamięć podręczna bramy scala je ze stroną i ostatecznie przesyła całą zawartość do
klienta.

Wszystko to działa w sposób przejrzysty na poziomie pamięci podręcznej bramy (czyli
poza aplikacją). Jak zobaczysz, gdy zdecydujesz się skorzystać ze znaczników ESI,
Symfony2 sprawia, że przetwarzanie tych znaczników nie wymaga wysiłku.

Używanie ESI w Symfony2
~~~~~~~~~~~~~~~~~~~~~~~

Najpierw trzeba włączyć obsługę ESI w konfiguracji aplikacji:

.. configuration-block::

    .. code-block:: yaml
       :linenos:

        # app/config/config.yml
        framework:
            # ...
            esi: { enabled: true }

    .. code-block:: xml
       :linenos:

        <!-- app/config/config.xml -->
        <framework:config ...>
            <!-- ... -->
            <framework:esi enabled="true" />
        </framework:config>

    .. code-block:: php
       :linenos:

        // app/config/config.php
        $container->loadFromExtension('framework', array(
            ...,
            'esi'    => array('enabled' => true),
        ));

Załóżmy teraz, że mamy względnie statyczną stronę, z wyjątkiem bloku aktualności
(*ang. news ticker*) na dole treści. Przy użyciu ESI można buforować ten blok
aktualności niezależnie od reszty strony.

.. code-block:: php
   :linenos:

    public function indexAction()
    {
        $response = $this->render('MyBundle:MyController:index.html.twig');
        // set the shared max age - which also marks the response as public
        $response->setSharedMaxAge(600);

        return $response;
    }

W tym przykładzie, bufor pełnej strony ma 10 minutowy czas życia. Następnie
dołączany jest w szablonie blok wiadomości, osadzając akcję. Realizowane jest to
przez helper ``render`` (zobacz :ref:`templating-embedding-controller` w celu
poznania szczegółów).

Ponieważ osadzana zawartość pochodzi z innej strony (lub kontrolera), Symfony2
stosuje standardowy helper ``render`` do skonfigurowania znaczników ESI:

.. configuration-block::

    .. code-block:: jinja
       :linenos:

        {# można również uzyć odniesienia do kontrolera #}
        {{ render_esi(controller('...:news', { 'max': 5 })) }}

        {# ... or a URL #}
        {{ render_esi(url('latest_news', { 'max': 5 })) }}

    .. code-block:: php
       :linenos:

        <?php echo $view['actions']->render(
            new ControllerReference('...:news', array('max' => 5)),
            array('renderer' => 'esi'))
        ?>

        <?php echo $view['actions']->render(
            $view['router']->generate('latest_news', array('max' => 5), true),
            array('renderer' => 'esi'),
        ) ?>

Wykorzystując renderowanie ``esi`` (poprzez funkcję ``render_esi`` Twiga) powiadamia
się Symfony2, że ta akcja powinna zostać zrenderowana jako znacznik ESI. Można zadać
pytanie, dlaczego użyliśmy helpera zamiast po prostu napisać sobie  znacznik ESI.
To dlatego, że używając helpera sprawiamy, że applikacja będzie działać nawet
w przypadku, gdy nie jest zainstalowana pamięć podręczna bramy.

Podczas używania domyślnej funkcji ``render`` (lub ustawienia renderowania na
``inline``), Symfony2 scala dołączone fragmenty z całą stroną przed jej wysłaniem
do klienta. Lecz jeśli używa się renderowania ``esi`` (tj. wywołania ``render_esi``)
i jeśli Symfony2 wykryje, że jest to kierowane do pamięci podręcznej bramy, która
obsługuje ESI, to generuje znacznik ``include`` ESI. Lecz jeśłi nie jest to pamięć
podręczna bramy lub pamięć taka nie obsługuje ESI, Symfony2 połączy tylko załączone
fragmenty treści ze stroną tak jak robi to wtedy, gdy używa ``render``.

.. note::

    Symfony2 wykrywa czy pamięć podręczna bramy wykrywa obsługę ESI z inną
    specyfikacją Akamaï i to jest jest obsługiwana "z pudełka" przez odwrotne proxy
    Symfony2.

Osadzone akcje mogą teraz określać własne zasady buforowania, całkowicie niezależnie
od strony właściwej.

.. code-block:: php
   :linenos:

    public function newsAction($max)
    {
        // ...

        $response->setSharedMaxAge(60);
    }

W ESI pełna strona będzie ważna przez 600 sekund, ale buforowany komponent wiadomości
tylko przez 60 sekund.

Podczas stosowania odniesienia do kontrolera znacznik ESI powinien odwoływać się do
osadzonej akcji jak do dostępnego adresu URL, tak więc pamięć podręczna może pobierać
wskazaną treść niezależnie od reszty strony. Symfony2 dba o wygenerowanie unikatowego
adresu URL dla każdego odniesienia do kontrolera i jest w stanie wyznaczyć każdy taki
adres właściwie, dzięki nasłuchowi (*ang. listner*), który musi być włączony w konfiguracji:

.. configuration-block::

    .. code-block:: yaml
       :linenos:

        # app/config/config.yml
        framework:
            # ...
            fragments: { path: /_fragment }

    .. code-block:: xml
       :linenos:

        <!-- app/config/config.xml -->
        <framework:config>
            <framework:fragments path="/_fragment" />
        </framework:config>

    .. code-block:: php
       :linenos:

        // app/config/config.php
        $container->loadFromExtension('framework', array(
            // ...
            'fragments' => array('path' => '/_fragment'),
        ));

Wielką zaletą renderowania ESI jest to, że można wykonać aplikację tak dynamiczną
jak jest to potrzebne i jednocześnie ograniczyć angażowanie aplikacji do niezbędnego
minimum.

.. tip::

    Nasłuch ogranicza się tylko do lokalnych adresów IP lub zaufanych serwerów
    pośredniczących.

.. note::

    Po rozpoczęciu pracy z ESI, trzeba pamiętać aby zawsze używać dyrektywy ``s-maxage``
    zamiast ``max-age``. Ponieważ przeglądarka zawsze tylko otrzymuje scalony zasób,
    więc nie jest świadoma istnienia komponentów i będzie przestrzegać dyrektywy
    ``max-age`` oraz buforować całą stronę, czego nie chcemy.

Helper ``render_esi`` obsługuje dwie użyteczne opcje:

* ``alt``: używaną jako atrybut ``alt`` w znaczniku ESI, który umożliwia określenie
  alternatywnego adresu URL używanego jeśli nie zostanie znaleziony zasób pod adresem
  podanym w ``src``;

* ``ignore_errors``: jeśli ma wartość ``true``, to atrybut ``onerror`` zostanie
  dodany do ESI z wartością ``continue``, wskazującą, że w przypadku awarii pamięć
  podręczna bramy usunie po cichu znacznik ESI.

.. index::
    single: pamięć podręczna; unieważnianie

.. _http-cache-invalidation:

Unieważnianie pamieci podręcznej
--------------------------------

      "Są tylko dwie trudne rzeczy w informatyce: unieważnianie pamięci podręcznej
      i nazywanie rzeczy". -- Phil Karlton    

Nigdy nie potrzeba unieważniać danych w pamięci podręcznej, bo unieważnianie jest
już uwzględnione w modelach buforowania HTTP. Jeśli używa się walidacji, to niczego
tu nie trzeba unieważniać z definicji, a jeśli używa się wygasania i trzeba unieważnić
zasób, to oznacza, że ustawiono zbyt odległą datę wygaśnięcia.

.. note::

    Ponieważ unieważnianie jest tematem specyficznym dla każdego typu odwrotnego
    proxy, to jeśli nie chcesz sobie zaprzątać głowy unieważnianiem, to możesz
    przełączyć się pomiędzy odwrotnymi proxy, bez zmieniania czegokolwiek w kodzie
    aplikacji.

Właściwie wszystkie odwrotne proxy zapewniają sposób opróżnienia danych z pamięci
podręcznej, ale należy unikać tego jak to tylko możliwe. Najbardziej standardowym
sposobem oczyszczenia pamięci podręcznej dla określonego adresu URL jest dostarczenie
do niej żądania ze specjalną metodą HTTP, ``PURGE``.

Oto jak można skonfigurować odwrotne proxy Symfony2 aby obsługiwało metodę HTTP
``PURGE``::

    // app/AppCache.php

    // ...
    use Symfony\Bundle\FrameworkBundle\HttpCache\HttpCache;
    use Symfony\Component\HttpFoundation\Request;
    use Symfony\Component\HttpFoundation\Response;

    class AppCache extends HttpCache
    {
        protected function invalidate(Request $request)
        {
            if ('PURGE' !== $request->getMethod()) {
                return parent::invalidate($request);
            }

            $response = new Response();
            if (!$this->getStore()->purge($request->getUri())) {
                $response->setStatusCode(404, 'Not purged');
            } else {
                $response->setStatusCode(200, 'Purged');
            }

            return $response;
        }
    }

.. caution::

    Trzeba koniecznie zabezpieczyć użycie metody HTTP ``PURGE``przed swobodnym
    użyciem, aby nie dopuścić do użycia jej przez przypadkowe osoby i niekontrolowanego
    usuwania danych z pamięci podręcznej.

Podsumowanie
------------

Symfony2 zostało zaprojektowane tak, aby przestrzegać zasad specyfikacji HTTP.
Buforowanie nie jest wyjątkiem. Opanowanie systemu buforowania Symfony2 oznacza
zapoznanie się z modelami buforowania HTTP i efektywnym ich wykorzystaniem.
Oznacza to z kolei, że zamiast poprzestać swoją edukację na dokumentacji Symfony2
i przykładach kodu, powinieneś jeszcze zaczerpnąć dodatkowej wiedzy o buforowaniu
HTTP i pamięci podręcznej bramy, takiej chociażby jak Varnish.

Dowiedz sie wiecej w Receptariuszu
----------------------------------

* :doc:`/cookbook/cache/varnish`

.. _`Things Caches Do`: http://tomayko.com/writings/things-caches-do
.. _`Cache Tutorial`: http://www.mnot.net/cache_docs/
.. _`Varnish`: https://www.varnish-cache.org/
.. _`Squid w trybie odwrotnego proxy`: http://wiki.squid-cache.org/SquidFaq/ReverseProxy
.. _`modelu wygasania`: http://tools.ietf.org/html/rfc2616#section-13.2
.. _`model walidacyjny`: http://tools.ietf.org/html/rfc2616#section-13.3
.. _`RFC 2616`: http://tools.ietf.org/html/rfc2616
.. _`HTTP Bis`: http://tools.ietf.org/wg/httpbis/
.. _`P4 - Conditional Requests`: http://tools.ietf.org/html/draft-ietf-httpbis-p4-conditional-12
.. _`P6 - Caching: Browser and intermediary caches`: http://tools.ietf.org/html/draft-ietf-httpbis-p6-cache-12
.. _`ESI`: http://www.w3.org/TR/esi-lang

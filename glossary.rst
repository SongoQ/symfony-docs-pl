:orphan:

Słownik
========

.. glossary::
   :sorted:

   Dystrybucja
        *Dystrybucja* (Distribution) jest paczką komponentów Symfony2,
        z wybranymi bundlami, sensowną strukturą katalogów,
        domyślną konfiguracją, oraz opcjonalnym systemem konfiguracji.

   Projekt
        *Projekt* (Project) jest katalogiem opakowanym w Aplikację, zbiorem bundli,
        bibliotekami zewnętrznymi, autoloaderem, oraz web front kontrolerem.

   Aplikacja
        *Aplikacja* (Application) jest katalogiem zawierającym *konfigurację* dla danego zbioru
        Bundli.

   Bundle
        *Bundle* (Bundle) jest katalogiem zawierającym zbiór plików (PHP, kaskadowych arkuszy
        stylów, JavaScripts, obrazków, ...) który *implementuje* pojedyńczą funkcjonalność
        (np. blog, forum, etc.). W Symfony2 (*prawie*) wszystko żyje w środku bundla.
        (zobacz :ref:`page-creation-bundles`)

   Front Kontroler
        *Front Kontroler* (Front Controller) jest małym plikiem PHP który żyje w głównym katalogu web
        Twojego projektu. Zazwyczaj *wszystkie* zapytania są obsługiwane przez
        wykonanie tego samego front kontrolera, którego zadaniem jest zainicjowanie
        aplikacji Symfony.

   Kontroler
        *Kontroler* (Controller) jest funkcją PHP która posiada całą niezbędną logikę aby zwrócić
        obiekt ``Response`` który reprezentuje konkretną stronę. Zazwyczaj
        routing jest zmapowany z kontrolerem, który używa informacji z zapytania
        do przetwarzania informacji, wykonywania akcji, a ostatecznie do zbudowania
        i zwrócenia obiektu ``Response``.

   Usługa
        *Usługa* (Service) jest ogólnym określeniem dla każdego obiektu PHP który wykonuje
        określone zadanie. Usługa jest zwykle używana "globalnie", jak obiekt połączenia
        z bazą danych lub obiekt dostarczający wiadomości e-mail. W Symfony2, usługi
        są często skonfigurowane i pobierane z kontenera usług. Aplikacja która posiada
        wiele oddzielnych usług jest nazywana `architekturą zorientowaną na usługi`_.
        
   Kontener Usług
        *Kontener Usług* (Service Container), zwany także *Dependency Injection Container*,
        jest specjalnym obiektem który zarządza instancjami usług wewnątrz aplikacji.
        Zamiast tworzyć usługi bezpośrednio, deweloper *instruuje* kontener usług
        (poprzez konfigurację) jak utworzyć usługi. Kontener usług zajmuje się "leniwym"
        inicjalizowaniem i wstrzykiwaniem zależnych usług. Zobacz rozdział 
        :doc:`/book/service_container`.

   Specyfikacja HTTP
        *Specyfikacja Http* (Http Specification) jest dokumentem opisującym
        "Hypertext Transfer Protocol" - zbiór zasad leżących u podstaw klasycznej
        komunikacji zapytanie-odpowiedź dla klient-serwer. Specyfikacja definiuje
        format używany dla zapytania (Request) oraz odpowiedzi (Response) jak i 
        możliwe nagłówki HTTP które mogą posiadać. Dla uzyskania większej ilości informacji
        przeczytaj artykuł Wikipedii `HTTP`_ mówiący o `HTTP 1.1 RFC`_.

   Środowisko
        Środowisko (Environment) jest ciągiem znaków (np. ``prod`` lub ``dev``) który
        przypisany jest dla specyficznego zestawu konfiguracyjnego. Ta sama aplikacja
        może być uruchamiana na tej samej maszynie używając różnej konfiguracji poprzez
        uruchamianie aplikacji w różnych środowiskach. Jest to użyteczne ponieważ pozwala
        pojedyńczej aplikacji posiadać środowisko ``dev`` dostosowane do debugowania oraz
        środowisko ``prod`` które jest zoptymalizowane pod kontem szybkości.

   Vendor
        *Vendor* jest dostawcą bibliotek PHP oraz bundli w tym także Symfony2.
        Pomimo skojarzenia tego słowa (przypomnienie tłumacza: vendor z 
        angielskiego to "sprzedawca") z kwestiami handlowymi, vendor w Symfony
        bardzo często (nawet zazwyczaj) dołącza bezpłatne oprogramowanie. Każda
        biblioteka którą chcesz dodać do projektu Symfony2 powinna znaleźć się w katalogu
        ``vendor``. Zobacz :ref:`Architektura: Używanie "Vendors" <using-vendors>`.

   Acme
       *Acme* jest prostą nazwą firmy użytej w demo Symfony oraz dokumentacjach.
       Jest użyta w przestrzeni nazw gdzie zwykle używana jest nazwa Twojej firmy
       (np. ``Acme\BlogBundle``).

   Akcja
       *Akcja* (Action) jest funkcją lub metodą PHP która jest wykonywana, dla przykładu,
       gdy przekazany routing zostanie dopasowany. Termin akcja jest synonimem z słowem
       *kontroler*, choć kontroler może również odnosić się do całej klasy PHP która
       zawiera kilka akcji. Zobacz :doc:`Rozdział o Kontrolerze </book/controller>`.
       
   Asset
       *Asset* jest nie-wykonywalnym, statycznym komponentem aplikacji webowej,
       włączając w to CSS, JavaScript, obrazki oraz wideo. Mogą być umiejscowione
       bezpośrednio w katalogu projektu ``web``, lub opublikowane z :term:`Bundle`
       do katalogu web poprzez użycie zadania ``assets:install`` z linii poleceń.

   Jądro
       *Jądro* (Kernel) jest rdzeniem Symfony2. Obiekt jądra obsługuje zapytania
       HTTP używając wszystkich bundli oraz bibliotek zarejestrowanych w nim.
       Zobacz :ref:`Architektura: Katalog app/ <the-app-dir>` oraz rozdział
       :doc:`/book/internals`.

   Firewall
       W Symfony2, *Firewall* nie ma nic do czynienia z siecią. Zamiast tego,
       definiuje mechanizmy uwierzytelniania (np. obsługuje proces ustalenia
       tożsamości użytkowników), dla całej aplikacji lub też tylko jego części
       Zobacz rozdział :doc:`/book/security`.

   YAML
       *YAML* jest rekurencyjnym akronim dla "YAML nie jest językiem znaczników".
       Jest lekkim, łatwym dla człowieka językiem serializacji danych szeroko używanym
       w plikach konfiguracyjnych Symfony2. Zobacz rozdział :doc:`/reference/YAML`.

.. _`architekturą zorientowaną na usługi`: http://wikipedia.org/wiki/Service-oriented_architecture
.. _`HTTP`: http://en.wikipedia.org/wiki/Hypertext_Transfer_Protocol
.. _`HTTP 1.1 RFC`: http://www.w3.org/Protocols/rfc2616/rfc2616.html

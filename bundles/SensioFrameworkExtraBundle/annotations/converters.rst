.. highlight:: php
   :linenothreshold: 2

.. index::
   single: adnotacje; @ParamConverter
   
@ParamConverter
---------------

Stosowanie
~~~~~~~~~~

Adnotacja **@ParamConverter** wywołuje konwertery do przekształcenia parametrów żądania
na obiekty. Obiekty te są przechowywane jako atrybuty żądania i jako takie mogą być
wstrzykiwane jako argumenty metod kontrolera::
   
   use Sensio\Bundle\FrameworkExtraBundle\Configuration\Route;
   use Sensio\Bundle\FrameworkExtraBundle\Configuration\ParamConverter;
   
   /**
   * @Route("/blog/{id}")
   * @ParamConverter("post", class="SensioBlogBundle:Post")
   */
   public function showAction(Post $post)
   {
   }
   
„Pod maską” dzieje się wiele rzeczy:

*  Konwerter próbuje pobrać obiekt ``SensioBlogBundle:Post`` z atrybutów żądania
   (atrybuty żądania dostarczane są z wieloznaczników trasy – w naszym przypadku
   ``id``);
*  Jeśli nie zostanie znaleziony obiekt ``Post``, to zostanie wygenerowana odpowiedź 404;
*  Jeśli znaleziony zostanie obiekt ``Post``, to zdefiniowany będzie nowy atrybut
   żądania ``post`` (dostępny poprzez ``$request->attributes->get('post'))``;
*  W przypadku występowania atrybutu w sygnaturze metody jest on automatycznie wstrzykiwany
   w kontroler, tak jak inne atrybuty żądania.
   
Jeśli używa się odgadywanego typu, tak jak w powyższym przykładzie, to w sumie można
nawet pominąć adnotację @ParamConverter::
   
   // automatic with method signature
   public function showAction(Post $post)
   {
   }
   
W celu wykrycia który konwerter działa dla parametru, uruchamiany jest następujący proces:

*  Jeśli w adnotacji jest jawnie określony konwerter, ``@ParamConverter(converter="name")``,
   to on właśnie jest wybierany.
*  W przeciwnym razie są wypróbowywane wszystkie zarejestrowane konwertery w kolejności
   ustalonego priorytetu. Do sprawdzenia czy konwerter może przekształcić parametr żądania
   wywoływana jest metoda ``supports()``. Jeśli zwraca ``true`` wywoływany jest ten konwerter.
   
Konwertery wbudowane
~~~~~~~~~~~~~~~~~~~~

Pakiet zawiera dwa wbudowane konwertery, konwerter Doctrine i konwerter DateTime.

Konwerter Doctrine
..................

Nazwa konwertera: *doctrine.orm*.

Konwerter Doctrine próbuje przekształcić atrybuty żądania na encje Doctrine pobieranych
z bazy danych. Możliwe są dwa różne podejścia:

*  Pobieranie obiektu wg klucza podstawowego;
*  Pobieranie obiektu wg jednego lub kilku pól, które zawierają unikalne wartości
   w bazie danych.

Poniższy algorytm określa która operacja zostanie zrealizowana:

*  Jeśli w trasie znajduje się parametr ``{id}``, to wyszukiwany jest obiekt
   wg klucza podstawowego.
*  Jeśli skonfigurowana jest opcja 'id' i pasuje ona do parametru trasy, to wyszukiwany
   jest obiekt wg klucza podstawowego.
*  Jeśli powyższe zasady nie mają zastosowania, to podejmowana jest próba odnalezienia
   jednej encji poprzez dopasowanie parametrów trasy do pól encji. Można kontrolować
   ten proces wyłączając parametry konfiguracyjne lub atrybut do mapowania nazwy pola.
   
Domyślnie konwerter Doctrine używa domyślnego menadżera encji. Można to skonfigurować
poprzez opcję ``entity_manager``::
   
   use Sensio\Bundle\FrameworkExtraBundle\Configuration\Route;
   use Sensio\Bundle\FrameworkExtraBundle\Configuration\ParamConverter;
   
   /**
   * @Route("/blog/{id}")
   * @ParamConverter("post", class="SensioBlogBundle:Post", options={"entity_manager" = "foo"})
   */
   public function showAction(Post $post)
   {
   }

Jeśli wieloznacznik nie ma takiej samej nazwy jak klucz główny, trzeba przekazać opcję ``id``::
   
   /**
   * @Route("/blog/{post_id}")
   * @ParamConverter("post", class="SensioBlogBundle:Post", options={"id" = "post_id"})
   */
   public function showAction(Post $post)
   {
   }
   
Pozwala to także mieć wiele konwertorów w jednej akcji::
   
   /**
   * @Route("/blog/{id}/comments/{comment_id}")
   * @ParamConverter("comment", class="SensioBlogBundle:Comment", options={"id" = "comment_id"})
   */
   public function showAction(Post $post, Comment $comment)
   {
   }

W powyższym przykładzie parametr *post* jest obsługiwany automatycznie, ale parametr
*comment* jest już skonfigurowany przez adnotację, ponieważ one razem nie mogą
przestrzegać domyślnej konwencji.

Jeśli chce się dopasować encję przy wykorzystaniu wielu pól, trzeba użyć odwzorowania::

   /**
   * @Route("/blog/{date}/{slug}/comments/{comment_slug}")
   * @ParamConverter("post", options={"mapping": {"date": "date", "slug": "slug"}})
   * @ParamConverter("comment", options={"mapping": {"comment_slug": "slug"}})
   */
   public function showAction(Post $post, Comment $comment)
   {
   }
   
Jeśli dopasowuje się encję używając kilka pól, ale chce się wyłączyć parametr trasy z kryterium::
   
   /**
   * @Route("/blog/{date}/{slug}")
   * @ParamConverter("post", options={"exclude": ["date"]})
   */
   public function showAction(Post $post, \DateTime $date)
   {
   }
   
Jeśli chce się określić metodę repozytorium aby używać jej do wyszukiwania encji
(na przykład, aby dodać złączenie do zapytania), można posłużyć się opcją
``repository_method``::
   
   /**
   * @Route("/blog/{id}")
   * @ParamConverter("post", class="SensioBlogBundle:Post", options={"repository_method" = "findWithJoins"})
   */
   public function showAction(Post $post)
   {
   }

Konwerter DateTime
..................

Nazwa konwertera: datetime

Konwerter datetime przekształca każdy atrybut trasy lub żądania do instancji datetime::
   
   /**
   * @Route("/blog/archive/{start}/{end}")
   */
   public function archiveAction(\DateTime $start, \DateTime $end)
   {
   }
   
Domyślnie akceptowany jest każdy format daty, który może być przetworzony przez
konstruktor *DateTime*. Można być bardziej rygorystycznym wykorzystując opcje::
   
   /**
   * @Route("/blog/archive/{start}/{end}")
   * @ParamConverter("start", options={"format": "Y-m-d"})
   * @ParamConverter("end", options={"format": "Y-m-d"})
   */
   public function archiveAction(\DateTime $start, \DateTime $end)
   {
   }
   
Tworzenie konwertera
~~~~~~~~~~~~~~~~~~~~

Wszystkie konwertery implementują `ParamConverterInterface`_::
   
   namespace Sensio\Bundle\FrameworkExtraBundle\Request\ParamConverter;
   
   use Sensio\Bundle\FrameworkExtraBundle\Configuration\ConfigurationInterface;
   use Symfony\Component\HttpFoundation\Request;
   
   interface ParamConverterInterface
   {
      function apply(Request $request, ConfigurationInterface $configuration);
      
      function supports(ConfigurationInterface $configuration);
   }

Metoda ``supports()`` zwraca wartość ``true``, gdy jest w stanie przekształcić daną
konfigurację (instancja ParamConverter).

Instancja *ParamConverter* ma trzy informacje o adnotacji:

*  ``name``: atrybut nazwy;
*  ``class``: atrybut nazwy klasy (może to być dowolny łańcuch tekstowy reprezentujący nazwę klasy);
*  ``options``: tablica opcji.

Metoda ``apply()`` jest wywoływana zawsze gdy jest obsługiwana konfiguracja.
W oparciu o atrybuty żądania, ustawia atrybut o nazwie ``$configuration->getName()``,
który przechowuje obiekt klasy ``$configuration->getClass()``.

Aby zarejestrować usługę konwertera trzeba dodać znacznik do swojej usługi:
   
.. configuration-block::
   
   .. code-block:: yaml
      :linenos:
      
      # app/config/config.yml
      services:
         my_converter:
            class:        MyBundle/Request/ParamConverter/MyConverter
            tags:
               - { name: request.param_converter, priority: -2, converter: my_converter }

   .. code-block:: xml
      :linenos:
      
      <service id="my_converter" class="MyBundle/Request/ParamConverter/MyConverter">
         <tag name="request.param_converter" priority="-2" converter="my_converter" />
      </service>

Można zarejestrować konwerter przez priorytet, przez nazwę (atrybut "converter")
lub przez oba te czynniki. Jeśli nie określi się priorytetu lub nazwy, to konwerter
zostanie dodany do stosu konwerterów  z priorytetem 0. Aby jawnie wyłączyć rejestrowanie
przez priorytet, trzeba ustawić ``priority="false"`` w znaczniku definicji.

.. tip::
   
   Używaj klasy ``DoctrineParamConverter`` jako szablonu dla własnych konwerterów.

.. _`ParamConverterInterface`: http://api.symfony.com/2.2/Sensio/Bundle/FrameworkExtraBundle/Request/ParamConverter/ParamConverterInterface.html
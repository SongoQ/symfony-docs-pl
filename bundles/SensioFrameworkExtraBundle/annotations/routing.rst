.. highlight:: php
   :linenothreshold: 2

.. index::
   single: adnotacje; @Route
   single: adnotacje; @Method
   
@Route i @Method
----------------

Stosowanie
~~~~~~~~~~

Adnotacja **@Route** odwzorowuje wzorzec trasy w kontrolerze::
   
   use Sensio\Bundle\FrameworkExtraBundle\Configuration\Route;
   
   class PostController extends Controller
   {
      /**
      * @Route("/")
      */
      public function indexAction()
      {
         // ...
      }
   }
   
Akcja *index* kontrolera *Post* jest teraz odzwzorowana na ścieżkę URL "/".
Jest to równoważne następującej konfiguracji YAML:

.. code-block:: yaml
   :linenos:
   
   blog_home:
      pattern:  /
      defaults: { _controller: SensioBlogBundle:Post:index }
      
Jak w każdym wzorcu trasy, tak i tu można określić wieloznaczniki, wymagania
i wartości domyślne::
   
   /**
   * @Route("/{id}", requirements={"id" = "\d+"}, defaults={"id" = 1})
   */
   public function showAction($id)
   {
   }

Można również określić wartość domyślną dla wieloznacznika z domyślną wartością PHP::
   
   /**
   * @Route("/{id}", requirements={"id" = "\d+"})
   */
   public function showAction($id = 1)
   {
   }
   
Można też dopasować więcej niż jedną ścieżkę URL definiując dodatkową adnotację @Route::
   
   /**
   * @Route("/", defaults={"id" = 1})
   * @Route("/{id}")
   */
   public function showAction($id)
   {
   }

.. _frameworkextra-annotations-routing-activation:
   
Aktywacja
~~~~~~~~~

W celu aktywowania trasy wymagane jest zaimportowanie wszelkich innych zasobów
trasowania (proszę zwrócic uwagę na typ zasobu):

.. code-block:: yaml
   :linenos:
   
   # app/config/routing.yml
   
   # import routes from a controller class
   post:
      resource: "@SensioBlogBundle/Controller/PostController.php"
      type:     annotation

Można również zaimportować cały katalog:

.. code-block:: yaml
   :linenos:
   
   # import routes from a controller directory
   blog:
      resource: "@SensioBlogBundle/Controller"
      type:     annotation
      
Trasy można "zamontować", jak każdy inny zasób, w zakresie określonego przedrostka
trasy:

.. code-block:: yaml
   :linenos:
   
   post:
      resource: "@SensioBlogBundle/Controller/PostController.php"
      prefix:   /blog
      type:     annotation
      
Nazwa trasy
~~~~~~~~~~~

Trasa określona poprzez adnotację *@Route* otrzymuje domyślną nazwą złożoną z nazwy
pakietu, nazwy kontrolera i nazwy akcji. W powyższym przykładzie byłaby to nazwa:
*sensio_blog_post_index*;

Do zmiany domyślnej nazwy trasy można użyć atrybutu *name*::
   
   /**
   * @Route("/", name="blog_home")
   */
   public function indexAction()
   {
      // ...
   }
   
Przedrostek trasy
~~~~~~~~~~~~~~~~~

Adnotacja @Route określona w klasie kontrolera definiuje przedrostek dla wszystkich
tras akcji w tej klasie::

   /**
   * @Route("/blog")
   */
   class PostController extends Controller
   {
      /**
      * @Route("/{id}")
      */
      public function showAction($id)
      {
      }
   }
   
Wyświetlana akcja jest teraz odwzorowana na wzorzec ``/blog/{id}``.

Metoda trasy
~~~~~~~~~~~~

Istnieje adnotacja skrótowa **@Method** określająca dopuszczalną dla trasy metodę HTTP.
Aby jej uzyć należy zaimportować przestrzeń nazw adnotacji Method::
   
   use Sensio\Bundle\FrameworkExtraBundle\Configuration\Route;
   use Sensio\Bundle\FrameworkExtraBundle\Configuration\Method;
   
   /**
   * @Route("/blog")
   */
   class PostController extends Controller
   {
      /**
      * @Route("/edit/{id}")
      * @Method({"GET", "POST"})
      */
      public function editAction($id)
      {
      }
   }
   
Edytowana akcja jest teraz odwzorowana na wzorzec ``/blog/edit/{id}`` jeśli
zastosowaną metodą HTTP jest GET albo POST.

Adnotacja @Method jest brana pod uwagę tylko wówczas, gdy akcja jest adnotowana
z użyciem @Route.

Kontroler jako usługa
~~~~~~~~~~~~~~~~~~~~~

Adnotacja *@Route* w klasie kontrolera może być również wykorzystywana do przypisania
klasy kontrolera do usługi tak, że rezolwer kontrolera będzie tworzył instancję
kontrolera przez pobieranie jej z kontenera DI zamiast wywoływanie
``new PostController()``::

   /**
   * @Route(service="my_post_controller_service")
   */
   class PostController extends Controller
   {
      // ...
   }
   

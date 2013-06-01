.. highlight:: php
   :linenothreshold: 2

.. index::
   single: adnotacje; @Cache
   
@Cache
------

Stosowanie
~~~~~~~~~~

Adnotacja **@Cache** ułatwia zdefiniowanie buforowania HTTP::
   
   use Sensio\Bundle\FrameworkExtraBundle\Configuration\Cache;

   /**
   * @Cache(expires="tomorrow", public="true")
   */
   public function indexAction()
   {
   }
   
Można również użyć adnotacji na poziomie klasy, aby zdefiniować buforowanie dla
wszystkich metod::
   
   /**
   * @Cache(expires="tomorrow", public="true")
   */
   class BlogController extends Controller
   {
   }
   
Gdy istnieje konflikt pomiędzy konfiguracją klasy a konfiguracją metody, to ostatnia
konfiguracja zastępuje poprzednią::
   
   /**
   * @Cache(expires="tomorrow")
   */
   class BlogController extends Controller
   {
       /**
      * @Cache(expires="+2 days")
      */
      public function indexAction()
      {
      }
   }

Atrybuty
~~~~~~~~

Oto lista akceptowanych atrybutów i ich równoważnych nagłówków HTTP:

   +---------------------------------+----------------------------------+
   | Adnotacja                       | Metoda odpowiedzi                |
   +---------------------------------+----------------------------------+
   | ``@Cache(expires="tomorrow")``  | ``$response->setExpires()``      |
   | ``@Cache(smaxage="15")``        | ``$response->setSharedMaxAge()`` |
   | ``@Cache(maxage="15")``         | ``$response->setMaxAge()``       |
   | ``@Cache(vary=["Cookie"])``     | ``$response->setVary()``         | 
   | ``@Cache(public="true")``       | ``$response->setPublic()``       |
   +-----------------------------+--------------------------------------+
   
.. note::
   
   Atrybut ``expires`` przyjmuje każdą prawidłową datę rozumianą przez funkcje
   PHP ``strtotime()``.

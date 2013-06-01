.. highlight:: php
   :linenothreshold: 2

.. index::
   single: pakiety; SensioFrameworkExtraBundle
   single: SensioFrameworkExtraBundle
   

SensioFrameworkExtraBundle
==========================

Pakiet *FrameworkBundle* Symfony2 implementuje podstawowy, ale solidny i elastyczny,
framework MVC. `SensioFrameworkExtraBundle`_ rozszerza go, dodając przyjazne konwencje
i adnotacje. Pozwala uzyskiwać bardziej zwięzły kod kontrolerów.

.. index::
   single: SensioFrameworkExtraBundle; instalacja

Instalacja
----------

`Pobierz`_ pakiet i umieść go w przestrzeni nazw Sensio\Bundle\. Następnie dołącz go
do klasy *Kernel*, podobnie jak czyni się to z innymi pakietami::

   public function registerBundles()
   {
      $bundles = array(
         ...
         
         new Sensio\Bundle\FrameworkExtraBundle\SensioFrameworkExtraBundle(),
      );
      
      ...
   }

Jeśli planuje się używanie lub tworzenie adnotacje dla kontrolerów, to trzeba zaktualizować
plik ``autoload.php`` dosając tam następującą linię::

   Doctrine\Common\Annotations\AnnotationRegistry::registerLoader(array($loader, 'loadClass'));


.. index::
   single: SensioFrameworkExtraBundle; konfiguracja
   
Konfiguracja
------------

Gdy pakiet zostanie zarejestrowany w klasie Kernel, to udostępnione zostaną domyślnie
wszystkie funkcje pakietu.

Domyślna konfiguracja jest następująca:

.. configuration-block::
   
   .. code-block:: yaml
      :linenos:
      
      sensio_framework_extra:
         router:  { annotations: true }
         request: { converters: true }
         view:    { annotations: true }
         cache:   { annotations: true }
      
   .. code-block:: xml
      :linenos:
      
      <!-- xmlns:sensio-framework-extra="http://symfony.com/schema/dic/symfony_extra" -->
      <sensio-framework-extra:config>
         <router annotations="true" />
         <request converters="true" />
         <view annotations="true" />
         <cache annotations="true" />
      </sensio-framework-extra:config>
      
   .. code-block:: php
      :linenos:
      
      // load the profiler
      $container->loadFromExtension('sensio_framework_extra', array(
         'router'  => array('annotations' => true),
         'request' => array('converters' => true),
         'view'    => array('annotations' => true),
         'cache'   => array('annotations' => true),
      ));

Można wyłączyć niektóre adnotacje i konwencje ustawiając jedną lub więcej opcji na *false*.

.. index::
   single: SensioFrameworkExtraBundle; adnotacje dla kontrolerów
   single: adnotacje
   
Adnotacje dla kontrolerów
-------------------------

Adnotacje są świetnym sposobem konfigurowania kontrolerów, począwszy od tras po konfigurację
pamieci podręcznej.

Chociaż adnotacje nie są natywną funkcjonalnością PHP, to posiadają wiele zalet w stosunku do
klasycznych metod konfiguracyjnych Symfony2:

* Kod i konfiguracja są w tym samym miejscu (klasa kontrolera);
* Są proste w nauce i stosowaniu;
* Są zwięzłe w pisaniu;
* Sprawiają, ze kontroler jest cienki (jego jedynym zadaniem staje się uzyskanie
  danych z modelu).
  
.. tip::
   
   Jeśli używa się klas widoków, to adnotacje są świetnym sposobem na unikniecie
   tworzenia klas widoków dla prostych i częstych przypadków użycia.
   
W pakiecie zdefiniowane są następujące adnotacje:

*  :doc:`@Route i @Method</bundles/SensioFrameworkExtraBundle/annotations/routing>`
*  :doc:`@ParamConverter</bundles/SensioFrameworkExtraBundle/annotations/converters>`
*  :doc:`@Template</bundles/SensioFrameworkExtraBundle/annotations/view>`
*  :doc:`@Cache</bundles/SensioFrameworkExtraBundle/annotations/cache>`

Ten przykład pokazuje w działaniu wszystkie dostępne adnotacje::
   
   use Sensio\Bundle\FrameworkExtraBundle\Configuration\Route;
   use Sensio\Bundle\FrameworkExtraBundle\Configuration\Cache;
   use Sensio\Bundle\FrameworkExtraBundle\Configuration\Template;
   use Sensio\Bundle\FrameworkExtraBundle\Configuration\ParamConverter;
   use Sensio\Bundle\FrameworkExtraBundle\Configuration\Method;
   
   /**
   * @Route("/blog")
   * @Cache(expires="tomorrow")
   */
   class AnnotController extends Controller
   {
       /**
      * @Route("/")
      * @Template
      */
      public function indexAction()
      {
         $posts = ...;
         
         return array('posts' => $posts);
      }

      /**
      * @Route("/{id}")
      * @Method("GET")
      * @ParamConverter("post", class="SensioBlogBundle:Post")
      * @Template("SensioBlogBundle:Annot:post.html.twig", vars={"post"})
      * @Cache(smaxage="15")
      */
      public function showAction(Post $post)
      {
      }
   }

Jako że metoda showAction spełnia kilka konwencji, to można pominąć niektóre adnotacje::
   
   /**
   * @Route("/{id}")
   * @Cache(smaxage="15")
   */
   public function showAction(Post $post)
   {
   }

Trasy muszą zostać zaimportowane aby były aktywne, tak jak wszystkie inne zasoby.
W celu poznania szczegółów przeczytaj rozdział ":ref:`annotated-routes-activation`".

Dokumentacja adnotacji
----------------------

.. toctree::
   :maxdepth: 1
   
   annotations/routing
   annotations/converters
   annotations/view
   annotations/cache    

.. _`SensioFrameworkExtraBundle`: https://github.com/sensio/SensioFrameworkExtraBundle
.. _`Pobierz`: http://github.com/sensio/SensioFrameworkExtraBundle
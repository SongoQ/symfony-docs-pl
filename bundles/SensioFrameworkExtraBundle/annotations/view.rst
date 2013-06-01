.. highlight:: php
   :linenothreshold: 2

.. index::
   single: adnotacje; @Template
   
@Template
---------

Stosowanie
~~~~~~~~~~

Adnotacja **@Template** kojarzy kontroler z nazwą szablonu::
   
   use Sensio\Bundle\FrameworkExtraBundle\Configuration\Template;
   
   /**
   * @Template("SensioBlogBundle:Post:show.html.twig")
   */
   public function showAction($id)
   {
      // get the Post
      $post = ...;
      
      return array('post' => $post);
   }
   
Podczas używania adnotacji *@Template*, kontroler powinien zwrócić tablicę parametrów
w celu przekazania do widoku zamiast obiektu *Response*.

.. note::
   
   Jeśli chcesz strumieniować swój szablon, to możesz wykonać to w następującej konfiguracji::
   
      /**
      * @Template(isStreamable=true)
      */
      public function showAction($id)
      {
         // ...
      }
      
.. tip::
   
   Jeśli akcja zwraca obiekt *Response*, adnotacja *@Template* jest po prostu ignorowana.

Jeśli szablon został nazwany wg nazwy kontrolera i akcji, a tak jest w powyższym przypadku,
to można nawet pominąć wartość adnotacji::
   
   /**
   * @Template
   */
   public function showAction($id)
   {
      // get the Post
      $post = ...;
      
      return array('post' => $post);
   }

.. note::

   Jeśli używa się PHP jako systemu szablonów, to trzeba wykonać to wyrażenie::
      
      /**
      * @Template(engine="php")
      */
      public function showAction($id)
      {
         // ...
      }
      
A jeśli tylko parametry przekazywane do szablonu są argumentami metody , można użyć
atrybutu ``vars`` zamiast zwracać tablicę. Jest to bardzo przydatne w kombinacji
z :doc:`adnotacją @ParamConverter</bundles/SensioFrameworkExtraBundle/annotations/converters>`::
   
   /**
   * @ParamConverter("post", class="SensioBlogBundle:Post")
   * @Template("SensioBlogBundle:Post:show.html.twig", vars={"post"})
   */
   public function showAction(Post $post)
   {
   }

co, dzięki konwencji, jest równoważne z nastęþującą konfiguracja::
   
   /**
   * @Template(vars={"post"})
   */
   public function showAction(Post $post)
   {
   }

Można to zrobić jeszcze bardziej prosto, jako że wszystkie argumenty są automatycznie
przekazywane do szablonu jeśli metoda zwraca ``null`` i nie jest określony żaden
atrybut ``vars``::
   
   /**
   * @Template
   */
   public function showAction(Post $post)
   {
   }

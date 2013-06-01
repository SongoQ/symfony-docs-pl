.. highlight:: php
   :linenothreshold: 2

.. index::
   single: formularze

Formularze
==========

Radzenie sobie z formularzami HTML jest jednym z najczęstszych (i trudnych) zadań,
z jakimi musi się uporać twórca aplikacji internetowych. Symfony2 integruje komponent
*Form*, który znacznie ułatwia pracę z formularzami. W tej części podręcznika zbudujemy
od podstaw skomplikowany formularz, ucząc się stopniowo najważniejszych funkcjonalności
biblioteki formularzy.

.. note::

   Komponent formularzy Symfony jest niezależną biblioteką, która może zostać użyta
   poza projektami Symfony2. Więcej informacji znajdziesz w dokumentacji
   `Symfony2 Form Component`_ dostępnej na Github.

.. index::
   single: formularze; utworzenie prostego formularza

Utworzenie prostego formularza
------------------------------

Załóżmy, że budujemy aplikację prostej listy zadań do zrobienia, która będzie
wykorzystywana do wyświetlania "zadań". Ponieważ nasi użytkownicy będą mieli
potrzebę edytowania i tworzenia zadań, to musimy zbudować formularz. Zanim zaczniemy,
skupimy swoją uwagę na ogólnej klasie ``Task``, która będzie reprezentować
i przechowywać dane dla pojedynczego zadania::

    // src/Acme/TaskBundle/Entity/Task.php
    namespace Acme\TaskBundle\Entity;

    class Task
    {
        protected $task;

        protected $dueDate;

        public function getTask()
        {
            return $this->task;
        }
        public function setTask($task)
        {
            $this->task = $task;
        }

        public function getDueDate()
        {
            return $this->dueDate;
        }
        public function setDueDate(\DateTime $dueDate = null)
        {
            $this->dueDate = $dueDate;
        }
    }

.. note::

   Jeśli chcesz kodować ten przykład, utwórz najpierw pakiet ``AcmeTaskBundle``,
   uruchamiając następujące polecenie (i zaakceptuj wszystkie domyślne opcje):

   .. code-block:: bash

        $ php app/console generate:bundle --namespace=Acme/TaskBundle

Klasa ta, jak dotychczas, nie ma nic wspólnego z Symfony lub jakąkolwiek biblioteką.
Jest ona zwykłym obiektem PHP, który bezpośrednio rozwiązuje problem wewnątrz naszej
aplikacji (tj. potrzebę reprezentowania zadania w naszej aplikacji). Pod koniec tego
rozdziału będziemy w stanie zgłosić dane do instancji ``Task`` (poprzez formularz HTM),
zweryfikować swoje dane i utrwalić je w bazie danych.

.. index::
   single: formularze; tworzenie formularza w kontrolerze

Budowanie formularza
~~~~~~~~~~~~~~~~~~~~

Teraz po utworzeniu klasy ``Task``, następnym krokiem jest utworzenie i zrenderowanie
rzeczywistego formularza HTML . W Symfony2 odbywa się to przez zbudowanie obiektu
formularza i zrenderowania formularza w szablonie. Na razie wszystko może być zrobione
wewnątrz kontrolera::

    // src/Acme/TaskBundle/Controller/DefaultController.php
    namespace Acme\TaskBundle\Controller;

    use Symfony\Bundle\FrameworkBundle\Controller\Controller;
    use Acme\TaskBundle\Entity\Task;
    use Symfony\Component\HttpFoundation\Request;

    class DefaultController extends Controller
    {
        public function newAction(Request $request)
        {
            // create a task and give it some dummy data for this example
            $task = new Task();
            $task->setTask('Write a blog post');
            $task->setDueDate(new \DateTime('tomorrow'));

            $form = $this->createFormBuilder($task)
                ->add('task', 'text')
                ->add('dueDate', 'date')
                ->getForm();

            return $this->render('AcmeTaskBundle:Default:new.html.twig', array(
                'form' => $form->createView(),
            ));
        }
    }

.. tip::

   Przykład ten pokazuje, jak zbudować formularz bezpośrednio w kontrolerze. Później,
   w rozdziale ":ref:`book-form-creating-form-classes`" dowiemy się, jak zbudować
   formularz w samodzielnej klasie, co jet zalecane, jako że formularz staje się
   wówczas możliwy do wielokrotnego wykorzystania.

Tworzenie formularza wymaga stosunkowo mało kodu, ponieważ obiekty formularza
Symfony2 są budowane w "konstruktorze formularzy". Celem konstruktora formularzy
jest umożliwienie prostego tworzenia "recept" na formularz i używanie ich do
rzeczywistego budowania formularzy.

W tym przykładzie dodaliśmy do formularza dwa pola, ``task`` i ``dueDate``,
odnoszące się do właściwości ``task`` i ``dueDate`` klasy ``Task``.
Mamy również do tych pól przypisany "typ" (np. ``text``, ``date``), który (między
innymi) określa jakie znaczniki formularza HTML są renderowane dla danego pola.

.. versionadded:: 2.3
    W Symfony 2.3 dodano obsługę przycisków zgłaszających (*submit*). Wcześniej
    trzeba było dodawać przyciski do formularza HTML ręcznie (jak w przykładzie
    w następnym rozdziale).

Symfony2 posiada wiele wbudowanych typów pól, które zostaną krótko omówione w
rozdziale :ref:`book-forms-type-reference`).

.. index::

   single: formularze; renderowanie

Renderowanie formularzy
~~~~~~~~~~~~~~~~~~~~~~~

Teraz, gdy formularz został utworzony, następnym krokiem jest jego zrederowanie.
Odbywa się to przez przekazanie specjalnego obiektu "widoku" formularza do szablonu
(zwróć uwagę na ``$form->createView()`` w kontrolerze powyżej) i użycie pomocniczych
funkcji formularza.

Przykład dla Symfony < 2.3:

.. configuration-block::

   .. code-block:: html+jinja
      :linenos:
      
      {# src/Acme/TaskBundle/Resources/views/Default/new.html.twig #}
      <form action="{{ path('task_new') }}" method="post" {{ form_enctype(form) }}>
         {{ form_widget(form) }}
         
         <input type="submit" />
      </form>
   
   .. code-block:: html+php
      :linenos:
      
      <!-- src/Acme/TaskBundle/Resources/views/Default/new.html.php -->
      <form action="<?php echo $view['router']->generate('task_new') ?>" method="post" <?php echo $view['form']->enctype($form) ?> >
         <?php echo $view['form']->widget($form) ?>
         
         <input type="submit" />
      </form>
      
.. note::

   Przykład ten zakłada, że stworzyliśmy trasę o nazwie ``task_new``, która wskazuje
   utworzony wcześniej kontroler ``AcmeTaskBundle:Default:new``.


Przykład dla Symfony 2.3:

.. configuration-block::

    .. code-block:: html+jinja
       :linenos:  

        {# src/Acme/TaskBundle/Resources/views/Default/new.html.twig #}

        {{ form(form) }}

    .. code-block:: html+php
       :linenos:  

        <!-- src/Acme/TaskBundle/Resources/views/Default/new.html.php -->

        <?php echo $view['form']->form($form) ?>

.. image:: /images/book/form-simple.png
    :align: center

.. note::

    W tym przykładzie założono, że formularz jest zgłaszany w żądaniu "POST" i na
    ten sam adres URL, który został w nim wyświetlony. Dowiesz się później, jak
    można zmienić metodę żądania i docelowy adres URL formularza.
    

To jest to! Przez wydrukowanie ``form_widget(form)``, zostanie w formularzu
zrenderowane każde pole, wraz z etykietą i komunikatem błędu (jeśli wystąpi).
Jest to proste lecz nie elastyczne (na razie). Zazwyczaj chce się zrenderować
każde pole formularza indywidualnie, dzięki czemu można kontrolować wygląd formularza.
Dowiesz się jak to zrobić w rozdziale ":ref:`form-rendering-template`".

Zanim przejdziemy dalej, zwróć uwagę na to, jak zostało zrenderowane pole wejściowe
``task``, mające wartość właściwości ``task`` obiektu ``$task`` (czyli "Write a blog
post"). Jest to pierwsza czynność formularza – pobranie danych z obiektu
i przetłumaczenie ich na format, który jest odpowiedni do odtworzenia w formularzu HTML.

.. tip::

   System formularza jest wystarczająco inteligentny aby uzyskać dostęp do wartości
   chronionych właściwości zadania poprzez metody ``getTask()`` i ``setTask()``
   w klasie ``Task``. Gdy właściwość nie jest publiczna, to musi się użyć metod
   "getter" i "setter", tak że komponent formularza może pobierać i zapisywać dane we
   właściwościach. Dla właściwości logicznych można użyć metody "isser" lub "hasser"
   (np. ``isPublished()`` lub ``hasReminder()``) wewnątrz akcesora pobierającego
   (np. ``getPublished()`` lub ``getReminder()``).

   .. versionadded:: 2.1
        W Symfony 2.1 dodano obsługę metod typu "hasser".

.. index::
  single: formularze; obsługa zgłoszeń formularza

Obsługa zgłoszeń formularza
~~~~~~~~~~~~~~~~~~~~~~~~~~~

Drugą czynnością formularza jest przełożenie przesłanych z powrotem danych
na właściwości obiektu. Aby tak się stało, trzeba powiązać z formularzem dane
przesłane przez użytkownika.

Przykład dla Symfony < 2.3:: 

    // ...
    use Symfony\Component\HttpFoundation\Request;

    public function newAction(Request $request)
    {
        // just setup a fresh $task object (remove the dummy data)
        $task = new Task();

        $form = $this->createFormBuilder($task)
            ->add('task', 'text')
            ->add('dueDate', 'date')
            ->getForm();

        if ($request->isMethod('POST')) {
            $form->bind($request);

            if ($form->isValid()) {
                // perform some action, such as saving the task to the database

                return $this->redirect($this->generateUrl('task_success'));
            }
        }

        // ...
    }

.. versionadded:: 2.1
    W Symfony 2.1 metoda ``bind`` została przerobiona i jest bardziej elastyczna.
    Akceptuje teraz surowe dane klienta (tak samo jak wcześniej) lub obiekt żądania
    Symfony. Jest to lepsze rozwiązanie od przestarzałej metody ``bindRequest``.

Teraz, po wysłaniu formularza, kontroler połączy przekazane dane z formularzem,
tłumaczą z powrotem dane na właściwości ``task`` i ``dueDate`` obiektu ``$task object``.
To wszystko dzieje się za sprawą metody ``bind()``.

.. note::

    Gdy jest wywoływana metoda ``bind()``, zgłoszone dane są transferowane bezpośrednio
    do wewnętrznego obiektu. Dzieje się tak niezależnie od tego, czy źródłowe dane
    są w rzeczywistości prawidłowe.


Przykład dla Symfony 2.3::

    // ...
    use Symfony\Component\HttpFoundation\Request;

    public function newAction(Request $request)
    {
        // just setup a fresh $task object (remove the dummy data)
        $task = new Task();

        $form = $this->createFormBuilder($task)
            ->add('task', 'text')
            ->add('dueDate', 'date')
            ->add('save', 'submit')
            ->getForm();

        $form->handleRequest($request);

        if ($form->isValid()) {
            // perform some action, such as saving the task to the database

            return $this->redirect($this->generateUrl('task_success'));
        }

        // ...
    }

.. versionadded:: 2.3
    W Symfony 2.3 została dodana metoda :method:`Symfony\Component\Form\FormInterface::handleRequest`.
    Poprzednio ``$request`` był przekazywany do metody ``submit`` - strategia, która jest już
    zdeprecjonowana i zostanie usunieta w Symfony 3.0. W celu poznania szczegółów tej metody proszę
    zapoznać się z :ref:`cookbook-form-submit-request`.


Kontroler ten akceptuje powszechny wzorzec obsługi formularzy i ma trzy możliwe tryby:

#. Podczas początkowego ładowania strony w przeglądarce, formularz jest tworzony
   i renderowany. Metoda :method:`Symfony\Component\Form\FormInterface::handleRequest`
   uznaje, że formularz nie został zgłoszony i nie robi nic. Jeśli formularz nie
   został zgłoszony, metoda :method:`Symfony\Component\Form\FormInterface::isValid`
   zwraca ``false``;

#. Gdy użytkownik zgłosi formularza, metoda
   :method:`Symfony\Component\Form\FormInterface::handleRequest` rozpoznaje to i
   natychmiast zapisuje zgłoszone dane z powrotem do właściwości ``task`` i ``dueDate``
   obiektu ``$task``. Następnie obiekt ten jest sprawdzany. Jeśli jest nieprawidłowy
   (walidacja jest omówiona w dalszym rozdziale), metoda
   :method:`Symfony\Component\Form\FormInterface::isValid` zwraca ``false``,
   więc formularz jest znowu renderowany razem ale ze wszystkimi błędami wykazanymi
   w walidacji;
   
   .. note::

       Można użyć metodę :method:`Symfony\Component\Form\FormInterface::isSubmitted`
       aby sprawdzić, czy formularz został zgłoszony, bez względu na to, czy zgłoszone
       dane są rzeczywiście prawidłowe, czy nie.

#. Gdy użytkownik zgłosi formularz z prawidłowymi danymi, zgłoszone dane są z powrotem
   zapisywane do formularza, ale tym razem metoda
   :method:`Symfony\Component\Form\FormInterface::isValid` zwraca ``true``.
   Teraz programista ma okazję aby wykonać kilka akcji, używając obiektu ``$task``
   (np. utrwalić dane w bazie danych), przed przekierowaniem uzytkownika do innej
   strony (np. do strony "Dziękujemy").
   
   .. note::
      
      Przekierowanie użytkownika po udanym zgłoszeniu formularza uniemożliwia
      użytkownikowi, by odświeżył i ponownie przesłał dane.   

.. index::
   single: formularze; wiele przycisków zgłaszających

.. _book-form-submitting-multiple-buttons:

Zgłaszanie formularzy z wieloma przyciskami
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. note::
   
   Rozdział ten dotyczy Symony 2.3.

.. versionadded:: 2.3
    W Symfony 2.3 dodano obsługę przycisków w formularzach.

Gdy formularz zawiera więcej niż jeden przycisk zgłaszający, to zachodzi potrzeba
informacji o tym, który przycisk został kliknięty, w celu dostosowania programu
w kontrolerze. Dodajmy do naszego formularza drugi przycisk z etykietą
"Zapisz i dodaj"::

    $form = $this->createFormBuilder($task)
        ->add('task', 'text')
        ->add('dueDate', 'date')
        ->add('save', 'submit')
        ->add('saveAndAdd', 'submit')
        ->getForm();

W kontrolerze użyjemy metody przycisku
:method:`Symfony\\Component\\Form\\ClickableInterface::isClicked`
w celu zapytania, czy kliknięty został przycisk "Zapisz i dodaj"::

    if ($form->isValid()) {
        // ... perform some action, such as saving the task to the database

        $nextAction = $form->get('saveAndAdd')->isClicked()
            ? 'task_new'
            : 'task_success';

        return $this->redirect($this->generateUrl($nextAction));
    }


.. index::
   single: formularze; walidacja

Walidacja formularza
--------------------

W poprzednim rozdziale dowiedziałeś się w jaki sposób można wysłać formularz
z danymi poprawnymi i niepoprawnymi. W Symfony2 walidacja jest stosowana do
wewnętrznego obiektu (np. ``Task``). Innymi słowami, nie chodzi o to czy
"formularz" jest prawidłowy, ale o to czy obiekt ``$task`` jest prawidłowy
po tym jak formularz zastosował dla niego zgłoszone dane. Wywołanie ``$form->isValid()``
jest skrótem, który odpytuje obiekt ``$task`` o poprawność danych.

Walidacja realizowana jest poprzez dodanie do klasy zestawu zasad (zwanych
ograniczeniami, *ang. constraints*). Aby zobaczyć jak to działa, dodamy ograniczenia
walidacyjne, tak aby pole ``task`` nie mogło być puste a pole ``dueDate`` nie mogło
być puste i musiało by być prawidłowym obiektem ``DateTime``.

.. configuration-block::

    .. code-block:: yaml
       :linenos:  

        # Acme/TaskBundle/Resources/config/validation.yml
        Acme\TaskBundle\Entity\Task:
            properties:
                task:
                    - NotBlank: ~
                dueDate:
                    - NotBlank: ~
                    - Type: \DateTime

    .. code-block:: php-annotations
       :linenos:  

        // Acme/TaskBundle/Entity/Task.php
        use Symfony\Component\Validator\Constraints as Assert;

        class Task
        {
            /**
             * @Assert\NotBlank()
             */
            public $task;

            /**
             * @Assert\NotBlank()
             * @Assert\Type("\DateTime")
             */
            protected $dueDate;
        }

    .. code-block:: xml
         
        <!-- Acme/TaskBundle/Resources/config/validation.xml -->
        <class name="Acme\TaskBundle\Entity\Task">
            <property name="task">
                <constraint name="NotBlank" />
            </property>
            <property name="dueDate">
                <constraint name="NotBlank" />
                <constraint name="Type">\DateTime</constraint>
            </property>
        </class>

    .. code-block:: php
       :linenos:  

        // Acme/TaskBundle/Entity/Task.php
        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints\NotBlank;
        use Symfony\Component\Validator\Constraints\Type;

        class Task
        {
            // ...

            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                $metadata->addPropertyConstraint('task', new NotBlank());

                $metadata->addPropertyConstraint('dueDate', new NotBlank());
                $metadata->addPropertyConstraint('dueDate', new Type('\DateTime'));
            }
        }

To jest to! Jeśli ponownie zgłosimy formularz z nieprawidłowymi danymi, to zobaczymy
w formularzu odpowiednio wydrukowane komunikaty o błędach.

.. _book-forms-html5-validation-disable:

.. sidebar:: Walidacja HTML5

   Wraz z HTML5, wiele przeglądarek może natywnie egzekwować pewne ograniczenia
   walidacji po stronie klienta. Najczęściej walidacja jest aktywowana przez
   wygenerowanie wymaganego atrybutu w polu, które tego wymaga. W przeglądarkach
   obsługujących HTML5 skutkuje to natywnym wyświetleniem przez przeglądarkę
   komunikatu o błędzie, jeśli dokonywana jest próba wysłania formularza z pustym polem.
   
   Renderowane formularze w pełni korzystają z tej nowej funkcjonalności, przez
   dodanie stosownym atrybutów HTML wyzwalających walidację. Walidacja po stronie
   klienta może jednak zostać wyłączona przez dodanie atrybutu ``novalidate``
   w znaczniku ``form`` lub ``formnovalidate`` dla znacznika przycisku zgłaszającego
   (*ang. submit button*). Jest to szczególnie przydatne, gdy chce się przetestować
   ograniczenia po stronie serwera, ale zablokowanych w przeglądarce (umożliwiając
   zgłaszanie przez nią pustych pól).

Walidacja jest bardzo zaawansowaną funkcjonalnością Symfony2 i opisana jest
w rozdziale :doc:`/book/validation`.

.. index::
   single: formularze; grupy walidacyjne

.. _book-forms-validation-groups:

Grupy walidacyjne
~~~~~~~~~~~~~~~~~

.. tip::

    Jeśli nie używasz :ref:`grup walidacyjnych<book-validation-validation-groups>`,
    to możesz pominąć ten rozdział.

Jeżeli stosuje się :ref:`grupy walidacyjne <book-validation-validation-groups>`
w obiekcie, to należy określić, które grupy (grupa) mają być użyte::

    $form = $this->createFormBuilder($users, array(
        'validation_groups' => array('registration'),
    ))->add(...);

Jeżeli tworzy się :ref:`klasy formularza<book-form-creating-form-classes>`
(dobra praktyka), to potrzeba dodać następujący kod do metody ``setDefaultOpt``::

    use Symfony\Component\OptionsResolver\OptionsResolverInterface;

    public function setDefaultOptions(OptionsResolverInterface $resolver)
    {
        $resolver->setDefaults(array(
            'validation_groups' => array('registration'),
        ));
    }

W obu przypadkach, do walidacji wewnętrznego obiektu zostaną użyte tylko
zarejestrowane grupy walidacyjne.

.. index::
   single: formularze; wyłączanie walidacji

Wyłączanie walidacji
~~~~~~~~~~~~~~~~~~~~

.. versionadded:: 2.3
    W Symfony 2.3 została dodana możliwość ustawienia opcji ``validation_groups``
    na ``false``, chociaż ustawienie w tej opcji pustej tablicy da ten sam wynik w
    poprzednich wersjach Symfony.

Czasami jest to przydatne w celu całkowitego powstrzymania walidacji formularza.
W takim przypadku można pominąć wywołanie metody
:method:`Symfony\\Component\\Form\\FormInterface::isValid` w kontrolerze.
Jeśli nie jest to wskazane (z powodów niżej wymienionych) , to można ewentualnie
ustawić opcję ``validation_groups`` na ``false`` lub pustą tablicę::

    use Symfony\Component\OptionsResolver\OptionsResolverInterface;

    public function setDefaultOptions(OptionsResolverInterface $resolver)
    {
        $resolver->setDefaults(array(
            'validation_groups' => false,
        ));
    }

Po ustawieniu opcję ``validation_groups`` na ``false`` lub pustą tablicę formularz
w dalszym ciagu będzie uruchamiał podstawowe sprawdzanie spójności, na przykład,
czy przesłany plik nie jest za duży lub czy istnieją zgłoszone pola. Jeśli chce
się wyłączyć walidację całkowicie, to należy usunąć wywołanie metody
:method:`Symfony\\Component\\Form\\FormInterface::isValid` z kontrolera.

.. index::
   single: formularze; grupy walidacyjne


Grupy walidacyjne oparte na zgłoszonych danych
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. versionadded:: 2.1
      W wersji 2.1 pojawiła się możliwość określenia wywołania zwrotnego
      lub domknięcia w ``validation_groups``.

Jeśli zachodzi potrzeba stworzenia jakiejś zaawansowanej logiki dla określenia
grup walidacyjnych (np. w oparciu o zgłaszane dane), to można ustawić opcję
``validation_groups`` na wywołanie zwrotne tablicy lub na domknięcie::

    use Symfony\Component\OptionsResolver\OptionsResolverInterface;

    public function setDefaultOptions(OptionsResolverInterface $resolver)
    {
        $resolver->setDefaults(array(
            'validation_groups' => array('Acme\\AcmeBundle\\Entity\\Client', 'determineValidationGroups'),
        ));
    }

Wywoła to statyczną metodę ``determineValidationGroups()`` klasy ``Client``
po powiązaniu formularza, ale przed wykonaniem walidacji. Obiekt formularza jest
przekazywany do tej metody jako argument (patrz następny przykład). Można również
określić całą logikę inline przy użyciu domknięcia::

    use Symfony\Component\Form\FormInterface;
    use Symfony\Component\OptionsResolver\OptionsResolverInterface;

    public function setDefaultOptions(OptionsResolverInterface $resolver)
    {
        $resolver->setDefaults(array(
            'validation_groups' => function(FormInterface $form) {
                $data = $form->getData();
                if (Entity\Client::TYPE_PERSON == $data->getType()) {
                    return array('person');
                } else {
                    return array('company');
                }
            },
        ));
    }

.. index::
   single: formularze; grupy walidacyjne

Grupy walidacyjne oparte na interaktywnym przycisku
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. note::
   
   Rozdział ten dotyczy Symfony 2.3.

.. versionadded:: 2.3
    W Symfony 2.3 została dodana obsługa przycisków.

Gdy formularz zawiera wiele przycisków zgłaszających (*submit*), można zmienić
grupę walidacyjną w zależności od tego, który przycisk został użyty do zgłoszenia
formularza. Na przykład, rozpatrzmy formularz kreatora, który pozwala na przechodzenie
do następnego kroku lub na cofnięcie się do poprzedniego kroku. Załóżmy również,
że po cofnięciu się do poprzedniego kroku dane formularza trzeba zapisać ale bez walidacji.

Najpierw potrzebujemy dodać do formularza dwa przyciski::

    $form = $this->createFormBuilder($task)
        // ...
        ->add('nextStep', 'submit')
        ->add('previousStep', 'submit')
        ->getForm();

Następnie skonfigurujemy przycisk powrotu do poprzedniego kroku aby uruchamiał
określoną grupe walidacyjną. W naszym przykładzie chcemy, aby walidacja została
wyłączona, tak więc ustawiamy opcję ``validation_groups`` na ``false``::

    $form = $this->createFormBuilder($task)
        // ...
        ->add('previousStep', 'submit', array(
            'validation_groups' => false,
        ))
        ->getForm();

Teraz formularz będzie pomijać swoje ograniczenia walidacyjne. Będą jeszcze sprawdzane
ograniczenia integralności, takie jak sprawdzenie czy przesyłany plik nie jest za
duży lub czy zgłoszono tekst w polu numerycznym.


.. index::
   single: formularze; wbudowane typy pól

.. _book-forms-type-reference:

Wbudowane typy pól
------------------

Symfony jest standardowo wyposażony w dużą grupę typów pól, która obejmuje wszystkie
popularne pola formularza i typy danych, z jakimi można mieć do czynienia:


.. include:: /reference/forms/types/map.rst.inc

Można również stworzyć własne typy pól. Temat ten jest omówiony w artykule
":doc:`Jak utworzyc własny typ pola formularza</cookbook/form/create_custom_field_type>`".

.. index::
   single: formularze; opcje typu pola

Opcje typu pola
~~~~~~~~~~~~~~~

Każdy typ pole ma kilka opcji, jakie mogą zostać użyte do skonfigurowania pola.
Na przykład, pole ``thedueDate`` jest obecnie renderowane jako 3 pola wyboru.
Jednak :doc:`pola daty</reference/forms/types/date>` mogą zostać skonfigurowane,
tak aby były renderowane jako pojedyncze pola tekstowe (gdzie użytkownik wpisuje
datę jako ciąg znaków)::

    ->add('dueDate', 'date', array('widget' => 'single_text'))

.. image:: /images/book/form-simple2.png
    :align: center

Każdy typ pola ma kilka różnych opcji, które mogą zostać do nich przekazane.
Wiele z nich jest określana dla danego typu pola. Szczegółowe informacje znajdziesz
w dokumentacji każdego typu pola.

.. sidebar:: Opcja ``required``

    Najbardziej popularną opcja jest ``required``, która może być zastosowana do
    każdego pola. Domyślnie opcja ``required`` jest ustawiona na ``true``, co oznacza,
    że przeglądarki obsługujace HTML5 będą stosowały walidację po stronie klienta,
    zgłaszając błąd, jeśli pole jest puste. Jeśli nie chcesz takiego zachowania,
    to ustaw opcję ``required`` na ``false`` lub wyłącz
    :ref:`walidację HTML5<book-forms-html5-validation-disable>`.

    Należy również pamiętać, że ustawienie opcji ``required`` na ``true`` **nie da
    rezultatu** w walidacji po stronie serwera. Innymi słowy, jeśli użytkownik wyśle
    pustą wartość dla tego pola (albo przykładowo posługuje się starą przeglądarką
    lub serwisem internetowym), to zostanie to zaakceptowane jako wartość prawidłowa,
    chyba że zostały użyte ograniczenia ``NotBlank`` lub ``NotNull``.

    Innym słowami, opcja ``required`` jest "super", ale prawdziwą walidację po
    stronie serwera należy stosować zawsze.

.. sidebar:: Opcja ``label``

    Etykietę dla pola formularza można ustawić opcją ``label``, którą można
    zastosować do każdego pola::

        ->add('dueDate', 'date', array(
            'widget' => 'single_text',
            'label'  => 'Due Date',
        ))

    Etykietę pola można również ustawić w szablonie generującym formularz
    – patrz poniżej.

.. index::
   single: formularze; odgadywany typ pola

.. _book-forms-field-guessing:

Odgadywany typ pola
-------------------

Teraz, gdy ostały dodane metadane walidacyjne do klasy ``Task``, Symfony wie już
trochę o swoich polach. Jeśli zezwoli się, to Symfony może "odgadywać" typ pola
i ustawiać go. W tym przykładzie Symfony może odgadywać typ pola z zasad walidacyjnych,
takich że zarówno pole ``task` jest zwykłym polem tekstowym jak i pole ``dueDate`` jest
polem datowym::

    public function newAction()
    {
        $task = new Task();

        $form = $this->createFormBuilder($task)
            ->add('task')
            ->add('dueDate', null, array('widget' => 'single_text'))
            ->getForm();
    }

"Odgadywanie" jest aktywowane gdy pominięty zostanie drugi argument metody ``add()``
(lub jeżeli przekaże się do niego wartość ``null``). Gdy przekaże się tablicę opcji
jako trzeci argument (w powyższym przykładzie zrobione jest to dla ``dueDate``),
opcje te zostaną zastosowane do pola o typie odgadywanym.

.. caution::

    Jeśli w formularzu używa się określonej grupy walidacyjnej, odgadywany typ
    pola będzie podczas odgadywania typu nadal uwzględniał wszystkie ograniczenia
    walidacyjne (w tym ograniczenia, które nie są częścią zastosowanych grup
    walidacyjnych).

.. index::
   single: formularze; odgadywany typ pola

Opcje odgadywanego typu pola
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Podczas odgadywania typu pola, Symfony może również próbować odgadnąć prawidłowe
wartości kilku opcji pola.

.. tip::

    Gdy te opcje są ustawione, pole będzie renderowane ze specjalnymi atrybutami
    HTML, które dostarczają walidację HTML5 po stronie serwera. Jednak to nie generuje
    równoważnych ograniczeń po stronie serwera (np. ``Assert\Length``). Chociaż trzeba
    będzie ręcznie dodać walidację po stronie serwera, te opcje typu pola można następnie
    odgadnąć na podstawie tej informacji.

* ``required``: Opcja ``required`` może zostać odgadnięta w oparciu o zasady walidacyjne
  (tzn. jest to opcja ``NotBlank`` lub ``NotNull``) lub o metadane Doctrine
  (tzn. jest to opcja ``nullable``). Jest to bardzo przydatne, bo walidacja po
  stronie serwera będzie automatycznie dopasowana do zasad walidacji.

* ``max_length``: Jeżeli pole jest jakimś polem tekstowym, to opcja ``max_length`` może zostać odgadnięta
  z ograniczeń walidacyjnych (jeżeli są użyte ``Length`` lub ``Range``) lub z metadanych
  Doctrine (poprzez długość pól).

.. note::

  Te opcje pola odgadywane są tylko gdy zastosuje się Symfony do odgadywania typu pola
  (tj. gdy pominie się lub przekaże wartość ``null`` jako drugi argument ``add()``).

Jeśli chce się zmienić jedną z odgadywanych wartości, to można zastąpić ją przez
przekazanie opcji w tablicy opcji pola::

    ->add('task', null, array('max_length' => 4))

.. index::
   single: formularze; renderowanie w szablonie

.. _form-rendering-template:

Renderowanie formularza w szablonie
-----------------------------------

Dotąd widzieliśmy jak cały formularz może być renderowany z użyciem tylko jednej
linii kodu. Oczywiście zwykle potrzeba o wiele więcej elastyczności przy renderowaniu
formularza:

- przykład dla Symfony < 2.3:

.. configuration-block::

    .. code-block:: html+jinja
       :linenos:  
         
        {# src/Acme/TaskBundle/Resources/views/Default/new.html.twig #}
        <form action="{{ path('task_new') }}" method="post" {{ form_enctype(form) }}>
            {{ form_errors(form) }}

            {{ form_row(form.task) }}
            {{ form_row(form.dueDate) }}

            {{ form_rest(form) }}

            <input type="submit" />
        </form>

    .. code-block:: html+php
       :linenos:  

        <!-- src/Acme/TaskBundle/Resources/views/Default/newAction.html.php -->
        <form action="<?php echo $view['router']->generate('task_new') ?>" method="post" <?php echo $view['form']->enctype($form) ?>>
            <?php echo $view['form']->errors($form) ?>

            <?php echo $view['form']->row($form['task']) ?>
            <?php echo $view['form']->row($form['dueDate']) ?>

            <?php echo $view['form']->rest($form) ?>

            <input type="submit" />
        </form>

Spójrzmy na każdą część tego kodu:

* ``form_enctype(form)`` - Jeśli co najmniej jedno pole jest polem ładowania pliku,
  wygeneruje to obowiązkowy atrybut ``enctype="multipart/form-data"``;

* ``form_errors(form)`` - Renderuje wszystkie błędy globalne dla całego formularza
  (błędy specyficzne dla pól są wyświetlane następnie przy każdym polu);

* ``form_row(form.dueDate)`` - Renderowanie etykiety, wszystkich błędy i widgety
  HTML formularza (np. ``dueDate``) znajdują się wewnątrz znacznika - domyślnie
  jest to div;

* ``form_rest(form)`` - Renderowanie pola, jakie nie zostało jeszcze wygenerowane.
  Jest zwykle dobrym pomysłem wywołanie tej funkcji pomocniczej na dole każdego
  formularza (w przypadku zapomnienia wyprowadzenia pola lub niechęci do ręcznego
  renderowania ukrytych pól). Ta funkcja pomocnicza jest również przydatna przy
  automatycznej :ref:`ochronie przed CSRF<forms-csrf>`.

- przykład dla Symony 2.3:

.. configuration-block::

    .. code-block:: html+jinja
       :linenos:

        {# src/Acme/TaskBundle/Resources/views/Default/new.html.twig #}
        {{ form_start(form) }}
            {{ form_errors(form) }}

            {{ form_row(form.task) }}
            {{ form_row(form.dueDate) }}

            <input type="submit" />
        {{ form_end(form) }}

    .. code-block:: html+php
       :linenos:

        <!-- src/Acme/TaskBundle/Resources/views/Default/newAction.html.php -->
        <?php echo $view['form']->start($form) ?>
            <?php echo $view['form']->errors($form) ?>

            <?php echo $view['form']->row($form['task']) ?>
            <?php echo $view['form']->row($form['dueDate']) ?>

            <input type="submit" />
        <?php echo $view['form']->end($form) ?>

W kodzie powyższego przykładu:

* ``form_start(form)`` - Renderuje znacznik rozpoczynający formularz;

* ``form_errors(form)`` - Renderuje wszystkie błędy globalne dla całego formularza
  (błędy specyficzne dla pól są wyświetlane następnie przy każdym polu);

* ``form_row(form.dueDate)`` - Renderuje etykietę, wszystkie błędy i widget formularza
  HTML dla danego pola (np. ``dueDate``), domyślnie wewnątrz bloku ``div``;

* ``form_end()`` - Renderuje znacznik zamykający formularza i wszystkie pola które
  nie zostały jeszcze wyrenderowane. Jest to przydatne dla renderowania ukrytych
  pól i znaczników automatycznej :ref:`ochrony przed atakami CSRF<forms-csrf>`.


Wiekszość prac jest wykonywana przez funkcje pomocniczą ``form_row``, która renderuje
etykietę, błędy i widget HTML formularza dla każdego pola wewnątrz znacznika,
domyślnie div. W rozdziale ":ref:`form-theming`" dowiesz się jak wyjście ``form_row``
może zostać dopasowane na różnych poziomach.

.. tip::

    Można uzyskać dostęp do bieżących danych formularza poprzez ``form.vars.value``:

    .. configuration-block::

        .. code-block:: jinja

            {{ form.vars.value.task }}

        .. code-block:: html+php

            <?php echo $view['form']->get('value')->getTask() ?>

.. index::
   single: formularze; ręczne renderowanie pól

Ręczne renderowanie każdego pola
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Funkcja pomocnicza ``form_row`` jest mocna ponieważ można bardzo szybko renderować
każde pole formularza (i można również dostosować znacznik używany w "wierszu").
Lecz ponieważ życie nie jest takie proste, to można również renderować calkowicie
ręcznie każde pole. Produkt końcowy tego co poniżej jest taki sam jak ten, jaki
używa funkcja ``form_row``:

- przykład dla Symfony < 2.3

.. configuration-block::

    .. code-block:: html+jinja
       :linenos:  

        {{ form_errors(form) }}

        <div>
            {{ form_label(form.task) }}
            {{ form_errors(form.task) }}
            {{ form_widget(form.task) }}
        </div>

        <div>
            {{ form_label(form.dueDate) }}
            {{ form_errors(form.dueDate) }}
            {{ form_widget(form.dueDate) }}
        </div>

        {{ form_rest(form) }}

    .. code-block:: html+php
       :linenos:  

        <?php echo $view['form']->errors($form) ?>

        <div>
            <?php echo $view['form']->label($form['task']) ?>
            <?php echo $view['form']->errors($form['task']) ?>
            <?php echo $view['form']->widget($form['task']) ?>
        </div>

        <div>
            <?php echo $view['form']->label($form['dueDate']) ?>
            <?php echo $view['form']->errors($form['dueDate']) ?>
            <?php echo $view['form']->widget($form['dueDate']) ?>
        </div>

        <?php echo $view['form']->rest($form) ?>

- Przykład dla Symfony 2.3:

.. configuration-block::

    .. code-block:: html+jinja

        {{ form_start(form) }}
            {{ form_errors(form) }}

            <div>
                {{ form_label(form.task) }}
                {{ form_errors(form.task) }}
                {{ form_widget(form.task) }}
            </div>

            <div>
                {{ form_label(form.dueDate) }}
                {{ form_errors(form.dueDate) }}
                {{ form_widget(form.dueDate) }}
            </div>

        <input type="submit" />

        {{ form_end(form) }}

    .. code-block:: html+php

        <?php echo $view['form']->start($form) ?>

            <?php echo $view['form']->errors($form) ?>

            <div>
                <?php echo $view['form']->label($form['task']) ?>
                <?php echo $view['form']->errors($form['task']) ?>
                <?php echo $view['form']->widget($form['task']) ?>
            </div>

            <div>
                <?php echo $view['form']->label($form['dueDate']) ?>
                <?php echo $view['form']->errors($form['dueDate']) ?>
                <?php echo $view['form']->widget($form['dueDate']) ?>
            </div>

            <input type="submit" />

        <?php echo $view['form']->end($form) ?>

Jeśli automatycznie generowana etykieta pola nie jest właściwa, to można określić
ją jawnie:

.. configuration-block::

    .. code-block:: html+jinja

        {{ form_label(form.task, 'Task Description') }}

    .. code-block:: html+php

        <?php echo $view['form']->label($form['task'], 'Task Description') ?>

Niektóre typy pól mają dodatkowo renderowane opcje, które mogą być przekazane do
widgetu. Opcje te są udokumentowane w dokumentacji każdego typu, ale jedną
z popularnniejszych opcji jest ``attr``, która umożliwia modyfikację atrybutów w
elemencie formularza. Poniżej dodamy klasę ``task_field`` do renderowanego wejściowego
pola tekstowego:

.. configuration-block::

    .. code-block:: html+jinja

        {{ form_widget(form.task, { 'attr': {'class': 'task_field'} }) }}

    .. code-block:: html+php

        <?php echo $view['form']->widget($form['task'], array(
            'attr' => array('class' => 'task_field'),
        )) ?>

Jeśli musisz renderować pola formularza "ręcznie", to możesz uzyskać dostęp do
poszczególnych wartości atrybutów pól, takich jak ``id``, ``name`` i ``label``.
Na przykład, aby pobrać wartość ``id``:

.. configuration-block::

    .. code-block:: html+jinja

        {{ form.task.vars.id }}

    .. code-block:: html+php

        <?php echo $form['task']->get('id') ?>

Aby pobrać wartość używaną w atrybucie nazwy pola formularza potrzeba użyć wartości
``full_name``:

.. configuration-block::

    .. code-block:: html+jinja

        {{ form.task.vars.full_name }}

    .. code-block:: html+php

        <?php echo $form['task']->get('full_name') ?>


Informacja o funkcjach szablonowych Twig
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Jeśli używasz Twig, to pełna informacja o funkcjach renderujących formularz jest
dostępna w :doc:`Reference Manual</reference/forms/twig_reference>`. Przeczytaj
to aby dowiedzieć się wszystkiego o dostępnych funkcjach pomocniczych i opcjach
jakie można użyć w każdej z nich.

.. index::
   single: Forms; Changing the action and method

.. _book-forms-changing-action-and-method:


Zmiana akcji i metody formularza
--------------------------------

.. note::
   
   Rozdział ten dotyczy Symfony 2.3

Dotychczas funkcja pomocnicza ``form_start()`` była używana do renderowania
początkowego znacznika formularza i zakładaliśmy, że każdy formularz jest
zgłaszany na ten sam adres co w żądaniu POST. Czasem zachodzi potrzeba zmiany
tych parametrów. Można to zrobić na kilka różnych sposobów. Jeżeli buduje się
formularz w kontrolerze, to można użyć metod ``setAction()`` i `setMethod()``::

    $form = $this->createFormBuilder($task)
        ->setAction($this->generateUrl('target_route'))
        ->setMethod('GET')
        ->add('task', 'text')
        ->add('dueDate', 'date')
        ->getForm();

.. note::

    W tym przykładzie zakłada się, że utworzono trasę o nazwie ``target_route``,
    która wskazuje na kontroler przetwarzający formularz.

W rozdziale :ref:`book-form-creating-form-classes` można dowiedzieć się, jak
przekształcić kod tworzący formularz na oddzielne klasy. W przypadku uzywania
zewnętrznej klasy formularza w kontrolerze można przekazać akcję i metodę jako
opcje formularza::

    $form = $this->createForm(new TaskType(), $task, array(
        'action' => $this->generateUrl('target_route'),
        'method' => 'GET',
    ));

Wreszcie można zastąpić akcję i metodę w szablonie przekazując je do funkcji
pomocniczych ``form()`` lub ``form_start()``:

.. configuration-block::

    .. code-block:: html+jinja
       :linenos:

        {# src/Acme/TaskBundle/Resources/views/Default/new.html.twig #}
        {{ form(form, {'action': path('target_route'), 'method': 'GET'}) }}

        {{ form_start(form, {'action': path('target_route'), 'method': 'GET'}) }}

    .. code-block:: html+php
       :linenos:

        <!-- src/Acme/TaskBundle/Resources/views/Default/newAction.html.php -->
        <?php echo $view['form']->form($form, array(
            'action' => $view['router']->generate('target_route'),
            'method' => 'GET',
        )) ?>

        <?php echo $view['form']->start($form, array(
            'action' => $view['router']->generate('target_route'),
            'method' => 'GET',
        )) ?>

.. note::

    Jeśli metodą formularza nie jest GET lub POST, ale PUT, PATCH lub DELETE, to
    Symfony2 wstawia ukryte pole z nazwą "_method", które przechowuje nazwę tej
    metody. Taki formularz będzie zgłaszany w zwykłym żądaniu POST, ale router
    Symfony2 jest w stanie wykryć parametr "_method" i zinterpretuje żądanie jako
    PUT, PATCH lub DELETE. Pproszę zapoznać się z artykułem
    ":doc:`/cookbook/routing/method_parameters`" w celu uzyskania więcej informacji.
    

.. index::
   single: formularze; tworzenie klas

.. _book-form-creating-form-classes:

Tworzenie klas formularza
-------------------------

Jak widać, formularz może być utworzony i używany bezpośrednio w kontrolerze.
Jednak lepszym rozwiązaniem jest zbudowanie formularza w oddzielnej, samodzielnej
klasie, która może być następnie używana wiele razy w różnych miejscach aplikacji.
Utwórzmy nową klasę, która będzie miejscem logiki dla zbudowania formularza zadania::

    // src/Acme/TaskBundle/Form/Type/TaskType.php
    namespace Acme\TaskBundle\Form\Type;

    use Symfony\Component\Form\AbstractType;
    use Symfony\Component\Form\FormBuilderInterface;

    class TaskType extends AbstractType
    {
        public function buildForm(FormBuilderInterface $builder, array $options)
        {
            $builder->add('task');
            $builder->add('dueDate', null, array('widget' => 'single_text'));
        }

        public function getName()
        {
            return 'task';
        }
    }

Nowa klasa zawiera wszystkie wskazówki potrzebne do utworzenia formularza zadania
(zwróć uwagę na to, że metoda ``getName()`` powinna zwracać unikalny identyfikator
dla „typu” tego formularza). Może być to stosowane do szybkiego zbudowania obietu
formularza w kontrolerze::

    // src/Acme/TaskBundle/Controller/DefaultController.php

    // add this new use statement at the top of the class
    use Acme\TaskBundle\Form\Type\TaskType;

    public function newAction()
    {
        $task = ...;
        $form = $this->createForm(new TaskType(), $task);

        // ...
    }

Umieszczenie logiki formularza we własnej klasie oznacza, że formularz może być
łatwo ponownie wykorzystany w jakimkolwiek miejscu projektu. Jest to najlepszy
sposób na tworzenie formularzy, ale wybór zależy tylko od Ciebie.

.. _book-forms-data-class:

.. sidebar:: Ustawienie ``data_class``

    Każdy formularz musi znać nazwę klasy, która przechowuje wewnetrzne dane
    (np. ``Acme\TaskBundle\Entity\Task``). Zazwyczaj jest ona odgadywana w oparciu
    o nazwę obiektu przekazywanego jako drugi argument metody ``createForm``
    (tj. obiektu klasu ``$task``). Później, gdy zaczniesz osadzanie formularza,
    nie będzie to wystarczające. Tak więc, choć nie zawsze jest to wystarczające,
    dobrym pomysłem jest jawnie określić opcję ``data_class`` przez dodanie
    następującego kodu do klasy typu formularza::

        use Symfony\Component\OptionsResolver\OptionsResolverInterface;

        public function setDefaultOptions(OptionsResolverInterface $resolver)
        {
            $resolver->setDefaults(array(
                'data_class' => 'Acme\TaskBundle\Entity\Task',
            ));
        }

.. tip::

    Kiedy odwzorowuje się formularze na obiekty, to z założenia odwzorowywane są
    wszystkie pola. Jakiekolwiek pola formularza, które nie istnieją w odwzorowywanym
    obiekcie, spowodują zrzucenie wyjątku.

    W przypadku gdy w formularzu potrzebne są dodatkowe pola (przykładowo, pole
    wyboru "Czy zgadzasz się z tymi warunkami?"), które nie będą odwzorowywane
    do wewnętrznego obiektu, potrzeba ustawić opcję ``mapped`` na ``false``::

        use Symfony\Component\Form\FormBuilderInterface;

        public function buildForm(FormBuilderInterface $builder, array $options)
        {
            $builder->add('task');
            $builder->add('dueDate', null, array('mapped' => false));
        }

    Dodatkowo, jeśli są jakieś pola w formularzu, które nie zostały dołączone
    w przesłanych danych, to pola te zostaną jawnie ustawione na null.

    Dane pola mogą być dostępne w kontrolerze przez::

        $form->get('dueDate')->getData();

.. index::
   pair: formularze; Doctrine

Formularze a Doctrine
---------------------

Celem formularza jest przetłumaczenie danych z obiektu (np. ``Task``) na formularz
HTML i przetłumaczenie przesłanych z powrotem przez użytkownika danych na oryginalny
obiekt. Jako taki, temat utrwalania obiektu ``Task`` w bazie danych jest całkowicie
niezależne od tematu formularza. Trzeba tu jednak zaznaczyć, że jeśli ma się
skonfigurowaną klasę ``Task`` to aby została ona utrwalana poprzez Doctrine
(czyli po dodaniu :ref:`metadanych odwzorowania<book-doctrine-adding-mapping>`),
to utrwalenie jej po zgłoszeniu formularza może zostać zrealizowane, gdy formularz
jest prawidłowy::

    if ($form->isValid()) {
        $em = $this->getDoctrine()->getManager();
        $em->persist($task);
        $em->flush();

        return $this->redirect($this->generateUrl('task_success'));
    }

Jeśli z jakiegoś powodu nie ma się dostępu do oryginalnego obiektu ``$task``,
można pobrać go z formularza::

    $task = $form->getData();

Więcej informacji znajdziesz w rozdziale :doc:`doctrine`.

Kluczowe jest zrozumienie, że gdy formularz jest związywany, to przesłane dane
są natychmiast transferowane do wewnętrznego obiektu. Jeśli chce się utrwalać te
dane, to po prostu trzeba utrwalić sam obiekt (który zawiera już przesłane dane).

.. index::
   single: formularze; formularze osadzone

Formularze osadzone
-------------------

Często zachodzi potrzeba zbudowania formularza zawierającego pola z różnych obiektów.
Na przykład, formularz rejestracji może zawierać dane należące do obiektu ``User``
jak również do wielu obiektów klasy ``Address``.
Na szczęście, jest to łatwe i naturalne z użyciem komponentu formularza.

Osadzanie pojedyńczych obiektów
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Załóżmy, że każdy obiekt ``Task`` należy do prostego obiektu ``Category``.
Rozpoczniemy oczywiście od utworzenia obiektu ``Category``::

    // src/Acme/TaskBundle/Entity/Category.php
    namespace Acme\TaskBundle\Entity;

    use Symfony\Component\Validator\Constraints as Assert;

    class Category
    {
        /**
         * @Assert\NotBlank()
         */
        public $name;
    }

Następnie dodamy nową właściwość ``category`` do klasy ``Task``::


    // ...

    class Task
    {
        // ...

        /**
         * @Assert\Type(type="Acme\TaskBundle\Entity\Category")
         */
        protected $category;

        // ...

        public function getCategory()
        {
            return $this->category;
        }

        public function setCategory(Category $category = null)
        {
            $this->category = $category;
        }
    }

Teraz, gdy aplikacja została zaktualizowana w celu odzwierciedlenia nowych wymagań,
utworzymy klasę formularza, tak aby obiekt ``Category`` mógł być modyfikowany przez
użytkownika::

    // src/Acme/TaskBundle/Form/Type/CategoryType.php
    namespace Acme\TaskBundle\Form\Type;

    use Symfony\Component\Form\AbstractType;
    use Symfony\Component\Form\FormBuilderInterface;
    use Symfony\Component\OptionsResolver\OptionsResolverInterface;

    class CategoryType extends AbstractType
    {
        public function buildForm(FormBuilderInterface $builder, array $options)
        {
            $builder->add('name');
        }

        public function setDefaultOptions(OptionsResolverInterface $resolver)
        {
            $resolver->setDefaults(array(
                'data_class' => 'Acme\TaskBundle\Entity\Category',
            ));
        }

        public function getName()
        {
            return 'category';
        }
    }

Ostatecznym celem jest umożliwienie, aby kategorie zadań mogły być modyfikowane
wewnątrz formularza zadania. Aby tego dokonać dodamy pole ``category`` do obiektu
``TaskType``, którego typem jest instancja nowej klasy ``CategoryType``:

.. code-block:: php
   :linenos:   
      
    use Symfony\Component\Form\FormBuilderInterface;

    public function buildForm(FormBuilderInterface $builder, array $options)
    {
        // ...

        $builder->add('category', new CategoryType());
    }

Pola z ``CategoryType`` mogą teraz być renderowane obok pól z ``TaskType``.
Aby aktywować walidację dla ``CategoryType`` trzeba dodać do ``TaskType`` opcję
``cascade_validation``::

    public function setDefaultOptions(OptionsResolverInterface $resolver)
    {
        $resolver->setDefaults(array(
            'data_class' => 'Acme\TaskBundle\Entity\Task',
            'cascade_validation' => true,
        ));
    }

Wyrenderujmy pola ``Category`` w ten sam sposób jak oryginalne pola ``Task``:

.. configuration-block::

    .. code-block:: html+jinja
       :linenos:

        {# ... #}

        <h3>Category</h3>
        <div class="category">
            {{ form_row(form.category.name) }}
        </div>

        {# ... #}

    .. code-block:: html+php
       :linenos:

        <!-- ... -->

        <h3>Category</h3>
        <div class="category">
            <?php echo $view['form']->row($form['category']['name']) ?>
        </div>

        <!-- ... -->

Gdy użytkownik zgłasza formularz, przesłane dane dla pól ``Category`` są używane
do konstruowania instancji ``Category``, która jest ustawiona na pole ``category``
instacji ``Task``.

Instancja ``Category`` jest dostępna naturalnie poprzez ``$task->getCategory()``
i może być utrwalona w bazie danych lub gdzie się tego potrzebuje.

Osadzanie kolekcji formularzy
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Można również osadzić kolekcję formularzy w jednym formularzu (wyobraź sobie
formularz ``Category`` z wieloma sub-formularzami ``Product``). Wykonuje się to
przez użycie typu pola ``collection``.

Więcej informacji można uzyskać w artykule
":doc:`Jak osadzoć kolekcję fotmularzy</cookbook/form/form_collections>` oraz
w :doc:`referencjach typu pola collection</reference/forms/types/collection>`.

.. index::
   single: formularze; dekorowanie
   single: formularze; dostosowywanie pól

.. _form-theming:

Dekorowanie formularza
----------------------

Każda część renderowanego formularza może być dostosowywana. Ma się możliwość
ustalania sposobu renderowania każdego „wiersza”, zmiany znacznika używanego do
wyświetlania błędów, a nawet dostosowania tego, jak powinien być renderowany
znacznik ``textarea``. Nie ma istotnych ograniczeń i można zastosować różne zmiany
w różnych miejscach.

Symfony używa szablonów do renderowania każdej części formularza, takiej jak
znaczniki ``label``, znaczniki ``input`` itd.

W Twig każdy "fragment" formularza jest reprezentowany przez blok Twiga.
Aby dostosować jakąś część formularza, wystarczy zastąpić określony blok

W PHP każdy "fragment" formularza jest renderowany przez indywidualny plik szablonowy.
Dla dostosowania jakiejś części formularza potrzeba zastąpić istniejący szablon nowym.

Aby zrozumieć jak to działa, dostosujemy fragment ``form_row`` i dodamy atrybut
``class`` do elementu ``div``, który otacza każdy wiersz. Aby to osiągnąć, utworzymy
nowy plik szablonowy, który zawierać będzie nowy znacznik:

.. configuration-block::

    .. code-block:: html+jinja
       :linenos:  

        {# src/Acme/TaskBundle/Resources/views/Form/fields.html.twig #}
        {% block form_row %}
        {% spaceless %}
            <div class="form_row">
                {{ form_label(form) }}
                {{ form_errors(form) }}
                {{ form_widget(form) }}
            </div>
        {% endspaceless %}
        {% endblock form_row %}

    .. code-block:: html+php
       :linenos:  

        <!-- src/Acme/TaskBundle/Resources/views/Form/form_row.html.php -->
        <div class="form_row">
            <?php echo $view['form']->label($form, $label) ?>
            <?php echo $view['form']->errors($form) ?>
            <?php echo $view['form']->widget($form, $parameters) ?>
        </div>

Fragment formularza ``form_row`` jest używany podczas renderowania większości pól
poprzez funkcje ``form_row``. W celu powiadomienia komponentu formularza, by używał
wyżej określony nowy fragment ``form_row``, trzeba dodać następujący kod w górnej
części szablonu:

.. configuration-block::

    .. code-block:: html+jinja

        {# src/Acme/TaskBundle/Resources/views/Default/new.html.twig #}
        {% form_theme form 'AcmeTaskBundle:Form:fields.html.twig' %}

        {% form_theme form 'AcmeTaskBundle:Form:fields.html.twig' 'AcmeTaskBundle:Form:fields2.html.twig' %}

        {{ form(form) }}

    .. code-block:: html+php

        <!-- src/Acme/TaskBundle/Resources/views/Default/new.html.php -->
        <?php $view['form']->setTheme($form, array('AcmeTaskBundle:Form')) ?>

        <?php $view['form']->setTheme($form, array('AcmeTaskBundle:Form', 'AcmeTaskBundle:Form')) ?>

        <?php echo $view['form']->form($form) ?>

Znacznik ``form_theme`` (w Twig) "importuje" fragmenty jkodu zdefiniowane w danym
szablonie i używa ich podczas renderowania formularza. Innymi słowami, gdy później
w szablonie wywoływana jest funkcja ``form_row``, to zostanie użyty blok ``form_row``
z własnego motywu (zamiast domyślnego bloku ``form_row``, który dostarczany jest
w Symfony).

Niestandardowy motyw nie musi zastępować wszystkich bloków. Blok, który nie został
zastąpiony podczas renderowania we własnej skórce, będzie pobrany z motywu globalnego
(zdefiniowanej na poziomie pakietu).

Jeśli stosuje się kilka niestandardowych motywów, to będą one wyszukiwane w kolejności
bloków podanych w motywie globalnym.

Aby dostosować jakiś fragment formularza, zachodzi potrzeba zastąpienia odpowiedniego
fragmentu. To jak dokładnie dowiedzieć się który blok lub plik powinien być zastąpiony
jest omówione w rozdziale następnym.

.. versionadded:: 2.1
   W wersji 2.1 została wprowadzona alternatywna składnia dla ``form_theme``.
   Akceptuje to każde prawidłowe wyrażenie Twig (najbardziej zauważalną różnicą
   jest użycie tablicy, która stosuje wiele motywów).

   .. code-block:: html+jinja
      :linenos:   

       {# src/Acme/TaskBundle/Resources/views/Default/new.html.twig #}

       {% form_theme form with 'AcmeTaskBundle:Form:fields.html.twig' %}

       {% form_theme form with ['AcmeTaskBundle:Form:fields.html.twig', 'AcmeTaskBundle:Form:fields2.html.twig'] %}


Więcej informacji można znaleźć w dokumencie
:doc:`Jak dostosować renderowany formularz</cookbook/form/form_customization>`.

.. index::
   single: formularze; nazewnictwo fragmentów formularza

.. _form-template-blocks:

Nazewnictwo fragmentów formularza
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

W Symfony, każda renderowana część formularza (elementy HTML formularza, etykiety itd.)
jest definiowana w szablonie podstawowym, który jest w Twig kolekcja bloków i kolekcją
plików szablonowych w PHP.

W Twig, każdy potrzebny blok jest zdefiniowany w pojedyńczym pliku szablonowym
(`form_div_layout.html.twig`_), który znajduje się wewnątrz `Twig Bridge`_.
Wewnątrz tego pliku, można zobaczyć każdy blok potrzebny do wygenerowania formularza
i każdego domyślnego typu pola.

W PHP, fragmenty są indywidualnymi plikami szablonowymi. Domyślnie są one umieszczone
w katalogu Resources/views/Form pakietu frameworka ( `zobacz na GitHub`_).

Każda nazwa fragmentu stosuje ten sam podstawowy wzorzec i składa się z dwóch części,
rozdzielonych znakiem podkreślenia (``_``). Oto kilka przykładów:

* ``form_row`` - używana przez ``form_row`` do renderowania większości pól;
* ``textarea_widget`` - używana przez ``form_widget`` do renderowania pól typu *textarea*;
* ``form_errors`` - używana przez ``form_errors`` do renderowania komunikatów błędów przy polach;

Każdy fragment stosuje nazwę według tego samego wzorca: *typ_fragment*.
Część ``typ`` odpowiada renderowanemu typowi pola (np. *textarea*, *checkbox*,
*date* itd.), natomiast część ``fragment`` odnosi się do rodzaju renderowanego
elementu formularza (np. *label*, *widget*, *errors* itd.). Domyślnie możliwe
są 4 fragmenty formularza (określane w części ``fragment``, które mogą być renderowane:

+------------+-----------------------+-----------------------------------------------------------+
| ``label``  | (np. ``form_label``)  | renderuje etykiety pola                                   |
+------------+-----------------------+-----------------------------------------------------------+
| ``widget`` | (np. ``form_widget``) | renderuje reprezentację pól HTML                          |
+------------+-----------------------+-----------------------------------------------------------+
| ``errors`` | (np. ``form_errors``) | renderuje komunikaty błędów pól                           |
+------------+-----------------------+-----------------------------------------------------------+
| ``row``    | (np. ``form_row``)    | renderuje cały wiersz formularza (label, widget i errors) |
+------------+-----------------------+-----------------------------------------------------------+

.. note::

    Istnieją jeszcze 3 inne części - ``rows``, ``rest`` i ``enctype``, ale bardzo
    rzadko sie je używa, jeżeli w ogóle.

Znając typ pola (np. *textarea*) i część formularza, którą chce się dostosować
(np. *widget*), można skonstruować nazwę fragmentu, który ma być przesłoniety
(tj. ``textarea_widget``).

.. index::
   single: formularze; dziedziczenie fragmentów formularza

Dziedziczenie fragmentów formularza
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

W niektórych przypadkach fragment, który chce się dostosować, może być wykazywany
jako zaginiony. Na przykład, w domyślnych szablonach dostarczany przez Symfony nie
występują fragmenty ``textarea_errors``. Tak więc jak są renderowane komunikaty
błędów dla pola *textarea*?

Odpowiedź brzmi: poprzez fragment ``form_errors``. Kiedy Symfony renderuje komunikaty
błędów dla typu *textarea*, to najpierw szuka fragmentu ``textarea_errors`` zanim
wykorzysta fragment ``form_errors``. Każdy typ pola ma swój typ nadrzędny (nadrzędny
typ *textarea* to *text*, a tego z kolei to *form*) i Symfony używa fragmentu dla
typu nadrzędnego jeśli fragment podstawowy nie istnieje.

Tak więc, aby przesłonić komunikaty błędów tylko dla pól *textarea*, trzeba skopiować
fragment ``form_errors``, zmienić jego nazwę na ``textarea_errors`` i odpowiednio
dostosować. Aby zastąpić domyślnie renderowane komunikaty błędów dla wszystkich pól,
trzeba skopiować i dostosować bezpośrednio fragment ``form_errors``.

.. tip::

    "Nadrzędny" typ każdego typu pola jest opisany w
    :doc:`informacji o typach pól</reference/forms/types>` opisanej dla każdego
    typu pola.

.. index::
   single: formularze; dekorowanie globalne formularza

Dekorowanie globalne formularza
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

W powyższym przykładzie użyliśmy ``form_theme helper`` (w Twig) do "zaimportowania"
dostosowanych fragmentów formularza do tego formularza. Można również powiadomić
Symfony aby importował przeróbki formularza w całym projekcie.

Twig
....

Aby automatycznie dołączyć własne bloki, z utworzonego wcześniej szablonu
``fields.html.twig``, do wszystkich szablonów, trzeba przerobić plik konfiguracyjny
aplikacji:

.. configuration-block::

    .. code-block:: yaml
       :linenos:  

        # app/config/config.yml
        twig:
            form:
                resources:
                    - 'AcmeTaskBundle:Form:fields.html.twig'
            # ...

    .. code-block:: xml
       :linenos:  

        <!-- app/config/config.xml -->
        <twig:config ...>
                <twig:form>
                    <resource>AcmeTaskBundle:Form:fields.html.twig</resource>
                </twig:form>
                <!-- ... -->
        </twig:config>

    .. code-block:: php
       :linenos:  

        // app/config/config.php
        $container->loadFromExtension('twig', array(
            'form' => array(
                'resources' => array(
                    'AcmeTaskBundle:Form:fields.html.twig',
                ),
            ),
            // ...
        ));


Teraz wszystkie bloki wewnątrz szablonu ``fields.html.twig`` są udostępniane
globalnie przy określaniu wyjścia formularza.

.. sidebar::  Dostosowywanie całego wyjścia formularza w jednym pliku w Twig

    W Twig można również dostosować blok formularza bezpośrednio w szablonie,
    gdy takie dostosowanie jest potrzebne:

    .. code-block:: html+jinja
       :linenos:  

        {% extends '::base.html.twig' %}

        {# import "_self" as the form theme #}
        {% form_theme form _self %}

        {# make the form fragment customization #}
        {% block form_row %}
            {# custom field row output #}
        {% endblock form_row %}

        {% block content %}
            {# ... #}

            {{ form_row(form.task) }}
        {% endblock %}

    Znacznik ``{% form_theme form _self %}`` umożliwia dostosowywanie bloków
    formularza bezpośrednio w szablonie, który będzie używał tych dostosowań.
    Użyj tej metody aby szybko wykonać dostosowanie wyjścia formularza, które
    będzie tylko czasem potrzebne w pojedynczym szablonie.

    .. caution::

        Funkcjonalność ``{% form_theme form _self %}`` działa tylko jeśli szablon
        rozszerza inny szablon. Jeżeli takiego szablonu nie ma, to należy wskazać
        form_theme do innego szablonu.

PHP
...

Aby automatycznie dołączyć własne szablony z wcześniej utworzonego katalogu
``Acme/TaskBundle/Resources/views/Form`` do wszystkich szablonów, trzeba poprawić
plik konfiguracyjny aplikacji:

.. configuration-block::

    .. code-block:: yaml
       :linenos:  

        # app/config/config.yml
        framework:
            templating:
                form:
                    resources:
                        - 'AcmeTaskBundle:Form'
        # ...


    .. code-block:: xml
       :linenos:  

        <!-- app/config/config.xml -->
        <framework:config ...>
            <framework:templating>
                <framework:form>
                    <resource>AcmeTaskBundle:Form</resource>
                </framework:form>
            </framework:templating>
            <!-- ... -->
        </framework:config>

    .. code-block:: php
       :linenos:  

        // app/config/config.php
        $container->loadFromExtension('framework', array(
            'templating' => array(
                'form' => array(
                    'resources' => array(
                        'AcmeTaskBundle:Form',
                    ),
                ),
            )
            // ...
        ));

Teraz wszystkie fragmenty wewnątrz katalogu ``Acme/TaskBundle/Resources/views/Form``
są dostępne globalnie przy określaniu wyjscia formularza.

.. index::
   single: formularze; ochrona przed CSRF

.. _forms-csrf:

Ochrona przed CSRF
------------------

CSRF (`Cross-Site Request Forgery`_), zwane też *fałszywym wywołaniem międzywitrynowym*,
to forma ataku, polegająca na tym, że nieświadomy użytkownik wykonuje żądanie
spreparowane przez osobę atakującą. Na szczęście ataki CSRF mogą być zablokowane
przez użycie w formularzach tokenów.

Dobrą wiadomością jest to, że domyślnie Symfony automatycznie osadza i sprawdza
tokeny CSRF. Oznacza to, że można skorzystać z ochrony przed CSRF nie robiąc nic.
W rzeczywistości każdy formularz użyty w tym rozdziale korzystał z tokenu
zabezpieczającego przed CSRF.

Ochrona przed CSRF działa w ten sposób, że do formularza dodawane jest ukryte pole,
o domyślnej nazwie ``_token``, którego wartość znana jest tylko serwerowi i klientowi.
To gwarantuje, że użytkownik (a nie jakiś inny podmiot) jest uprawniony do przesłania
danych formularza. Symfony automatycznie sprawdza obecność i rzetelność tego tokenu.

Pole ``_token`` jest ukrytym polem i zostanie automatycznie wygenerowane, jeśli
dołączy się w szablonie funkcję ``form_rest()``. Funkcja ta zapewnia, że na wyjściu
znajdują się wszystkie nie renderowane pola.

Token CSRF może zostać dopasowany w konfiguracji formularza. Przykładowo::

    use Symfony\Component\OptionsResolver\OptionsResolverInterface;

    class TaskType extends AbstractType
    {
        // ...

        public function setDefaultOptions(OptionsResolverInterface $resolver)
        {
            $resolver->setDefaults(array(
                'data_class'      => 'Acme\TaskBundle\Entity\Task',
                'csrf_protection' => true,
                'csrf_field_name' => '_token',
                // a unique key to help generate the secret token
                'intention'       => 'task_item',
            ));
        }

        // ...
    }


Aby wyłączyć ochronę przed CSRF, trzeba ustawić opcję ``csrf_protection`` na ``false``.
Dostosowanie może być również wykonane globalnie dla całego projektu. Więcej informacji
znajdziesz w rozdziale :ref:`Informacje o konfiguracji formularza<reference-framework-form>`.

.. note::

    Opcja ``intention`` jest opcjonalna, ale znacznie zwiększa bezpieczeństwo
    generowanego tokenu przez jego idywidualizację dla każdego formularza.

.. index::
   single: formularze; bez klasy

Stosowania formularza bez klasy
-------------------------------

W większości przypadków formularz jest związany z obiektem a pola formularza
pobierają i przechowują swoje dane we właściwościach obiektu. Jest to dokładnie
to, co widzieliśmy do tej pory w tej części podręcznika.

Ale czasami możemy chcieć stosować formularz bez klasy i zwracać tablicę zgłoszonych
danych. Jest to w rzeczywistości bardzo proste::

    // make sure you've imported the Request namespace above the class
    use Symfony\Component\HttpFoundation\Request;
    // ...

    public function contactAction(Request $request)
    {
        $defaultData = array('message' => 'Type your message here');
        $form = $this->createFormBuilder($defaultData)
            ->add('name', 'text')
            ->add('email', 'email')
            ->add('message', 'textarea')
            ->getForm();

            if ($request->isMethod('POST')) {
                $form->bind($request);

                // data is an array with "name", "email", and "message" keys
                $data = $form->getData();
            }

        // ... render the form
    }

Domyślnie, formularz w rzeczywistości zakłada, że chce się pracować z tablicą danych
zamiast z obiektem. Istnieją dwa sposoby zmiany tego zachowania i związania formularza
z obiektem: 

#. Przekazanie obiekt podczas tworzenia formularza (jako pierwszy argument metody
   ``createFormBuilder`` lub jako drugi argument metody ``createForm``);

#. Zadeklarowanie opcji ``data_class`` w formularzu.


Jeśli nie zrobi się którejś z powyższych czynności, to formularz zwróci dane jako
tablicę. W tym przykładzie, ponieważ ``$defaultData`` nie jest obiektem (i nie jest
ustawiona opcja ``data_class``), to ``$form->getData()`` ostatecznie zwraca tablicę.

.. tip::

    Można również uzyskać bezpośredni dostęp do wartości POST (w tym przypadku "name")
    za pomocą obiektu żądania, podobnie do tego::

        $this->get('request')->request->get('name');

    Należy pamiętać, że w większości przypadków lepszym wyborem jest użycie metody
    ``getData()``, ponieważ zwracane są dane (zazwyczaj obiekt) po tym jak zostały
    one przekształcone przez framework formularza.

Dodawanie walidacji
~~~~~~~~~~~~~~~~~~~

Jedynym brakującym jeszcze elementem jest walidacja. Zazwyczaj, gdy wywołuje się
metodę ``$form->isValid()``, to obiekt zostaje sprawdzony przez renderowanie
ograniczenia, które zastosowało się dla tej klasy. Jeśli formularz jest powiązany
z obiektem (np. przez użycie opcji ``data_class`` lub przekazanie obiektu do formularza),
jest to podejście najlepsze. Zobacz :doc:`validation`
w celu poznania szczegółów.

.. _form-option-constraints:

Ale jeśli nie powiąże się obiektu z formularzem i zamiast tego pobiera się prostą
tablice zgłoszonych danych, to jak można dodać ograniczenia dla danych formularza?

Odpowiedzią jest ustawienie sobie ograniczeń i dołączenie ich do indywidualnych pól.
Ogólne podejście jest omówione trochę więcej w części :ref:`Walidacja<book-validation-raw-values>`,
ale oto krótki przykład:

.. versionadded:: 2.1
   Nowością w Symfony 2.1 jest opcja ``constraints``, która przyjmuje pojedyncze
   ograniczenie lub tablicę ograniczeń (przed 2.1 opcje były wywoływane przez
   ``idation_constraint`` i akceptowane były tylko pojedyncze ograniczenia) .
   
.. code-block:: php
   :linenos:   

    use Symfony\Component\Validator\Constraints\Length;
    use Symfony\Component\Validator\Constraints\NotBlank;

    $builder
       ->add('firstName', 'text', array(
           'constraints' => new Length(array('min' => 3)),
       ))
       ->add('lastName', 'text', array(
           'constraints' => array(
               new NotBlank(),
               new Length(array('min' => 3)),
           ),
       ))
    ;

.. tip::

    Jeśli używa się grup walidacyjnych, to zachodzi potrzeba albo odwoływania się
    do ``Defaultgroup`` podczas tworzenia formularza lub ustawienia właściwej grupy
    na dodawanym ograniczeniu.
    
.. code-block:: php
   
    new NotBlank(array('groups' => array('create', 'update'))
    
    
Wnioski końcowe
---------------

Teraz już poznałeś wszystkie klocki niezbędne do budowy złożonych i funkcjonalnych
formularzy w swojej aplikacji. Podczas budowania formularza należy pamiętać, że
pierwszym celem formularza jest przetłumaczenie danych z obiektu formularza (``Task``)
na formularz HTML, tak aby użytkownik mógł modyfikować dane. Drugim celem formularza
jest przejęcie danych przesłanych przez użytkownikai ponownego naniesienie ich na
obiekt.

Jest jeszcze przed Tobą wiele nauki o nieomówionych tu zagadnieniach z zakresu
formularzy, takich jak :doc:`obsługa ładowania plików w Doctrine</cookbook/doctrine/file_uploads>`
czy jak tworzyć formularz, w którym może być dodawana dynamicznie jakaś liczba
sub-formularzy (np. lista zadań do wykonania, gdzie można udostępnić dodawanie pól
poprzez Javascript przed wysłaniem danych). Przeczytaj artykuły o tym zagadnieniu
w Receptariuszu. Ponadto trzeba by poznać :doc:`dokumentację typów pól</reference/forms/types>`,
która zawiera przykłady używania typów pól i ich opcji.

Dalsza lektura
--------------

* :doc:`/cookbook/doctrine/file_uploads`
* :doc:`File Field Reference </reference/forms/types/file>`
* :doc:`Creating Custom Field Types </cookbook/form/create_custom_field_type>`
* :doc:`/cookbook/form/form_customization`
* :doc:`/cookbook/form/dynamic_form_modification`
* :doc:`/cookbook/form/data_transformers`

.. _`Symfony2 Form Component`: https://github.com/symfony/Form
.. _`DateTime`: http://php.net/manual/en/class.datetime.php
.. _`Twig Bridge`: https://github.com/symfony/symfony/tree/2.2/src/Symfony/Bridge/Twig
.. _`form_div_layout.html.twig`: https://github.com/symfony/symfony/blob/2.2/src/Symfony/Bridge/Twig/Resources/views/Form/form_div_layout.html.twig
.. _`Cross-site request forgery`: http://en.wikipedia.org/wiki/Cross-site_request_forgery
.. _`zobacz na GitHub`: https://github.com/symfony/symfony/tree/2.2/src/Symfony/Bundle/FrameworkBundle/Resources/views/Form

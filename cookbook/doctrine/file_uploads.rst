Jak obsłużyć Wysyłanie Plików z Doctrine
========================================

Obsługa wysyłania plików z encjami Doctrine nie różni się niczym innym od obsługi
jakiegokolwiek innego wysyłania plików. Innymi słowy, możesz przenieś swój plik w
kontrolerze po wysłaniu formularza. Aby zobaczyć przykłady użycia, zobacz
stronę :doc:`typy plików</reference/forms/types/file>`.

Jeśli się zdecydujesz, możesz także zintegrować wysyłanie pliku z cyklem życia
encji (np. dodawanie, edycja oraz usuwanie). W tym przypadku, gdy Twoja encja
zostaje utworzona, zedytowana lub usunięta z Doctrine, proces przesyłania pliku oraz
usuwania go zostanie wywołany automatycznie (bez konieczności robienia czegokolwiek
w kontrolerze);

Aby to zrobić, musisz zadbać o kilka szczegółów, o których opowiemy w tym rozdziale.

Podstawowe Ustawienia
---------------------

Po pierwsze, utwórz prostą klasę Encji Doctrine::

    // src/Acme/DemoBundle/Entity/Document.php
    namespace Acme\DemoBundle\Entity;

    use Doctrine\ORM\Mapping as ORM;
    use Symfony\Component\Validator\Constraints as Assert;

    /**
     * @ORM\Entity
     */
    class Document
    {
        /**
         * @ORM\Id
         * @ORM\Column(type="integer")
         * @ORM\GeneratedValue(strategy="AUTO")
         */
        public $id;

        /**
         * @ORM\Column(type="string", length=255)
         * @Assert\NotBlank
         */
        public $name;

        /**
         * @ORM\Column(type="string", length=255, nullable=true)
         */
        public $path;

        public function getAbsolutePath()
        {
            return null === $this->path ? null : $this->getUploadRootDir().'/'.$this->path;
        }

        public function getWebPath()
        {
            return null === $this->path ? null : $this->getUploadDir().'/'.$this->path;
        }

        protected function getUploadRootDir()
        {
            // the absolute directory path where uploaded documents should be saved
            return __DIR__.'/../../../../web/'.$this->getUploadDir();
        }

        protected function getUploadDir()
        {
            // get rid of the __DIR__ so it doesn't screw when displaying uploaded doc/image in the view.
            return 'uploads/documents';
        }
    }

Encja o nazwie ``Document`` jest związana z plikiem. Zmienna ``path`` przechowuje
względną ścieżkę do pliku oraz jest przechowywana w bazie danych. Metoda ``getAbsolutePath()`
zwraca absolutną ścieżkę do pliku, natomiast metoda ``getWebPath()`` zwraca ścieżkę w stosunku
do katalogu ``web``, która to może zostać użyta w szablonie do np. linkowania do wysłanego pliku.

.. tip::

    Jeśli nie robiłeś tego wcześniej, zapewne powinieneś przeczytać :doc:`file</reference/forms/types/file>`
    aby zrozumieć jak działa wysyłanie plików.

.. note::

    Jeśli używasz adnotacji aby określić zasady adnotacji (tak jak w tym przykładzie),
    upewnij się czy masz włączoną obsługę walidacji przez adnotacje
    (zobacz :ref:`validation configuration<book-validation-configuration>`).

Aby obsłużyć wysyłanie pliku w formularzu, użyj "wirtualnego" pola ``file``.
Jeśli przygotowujesz swój formularz bezpośrednio w kontrolerze, może on wyglądać
tak::

    public function uploadAction()
    {
        // ...

        $form = $this->createFormBuilder($document)
            ->add('name')
            ->add('file')
            ->getForm()
        ;

        // ...
    }

Następnie, utwórz zmienną o takiej samej w klasie ``Document`` oraz dodaj kilka
zasad walidacji::

    // src/Acme/DemoBundle/Entity/Document.php

    // ...
    class Document
    {
        /**
         * @Assert\File(maxSize="6000000")
         */
        public $file;

        // ...
    }

.. note::

    Gdy używasz ograniczenia ``File``, Symfony2 automatycznie odgadnie że
    te pole formularza jest polem wysyłanego pliku. Dlatego nie musisz
    robić tego bezpośrednio podczas tworzenia formularza - jak powyżej
    (``->add('file')``).

Poniższy kontroler pokaże Ci jak wygląda obsługa całego procesu::

    use Acme\DemoBundle\Entity\Document;
    use Sensio\Bundle\FrameworkExtraBundle\Configuration\Template;
    // ...

    /**
     * @Template()
     */
    public function uploadAction()
    {
        $document = new Document();
        $form = $this->createFormBuilder($document)
            ->add('name')
            ->add('file')
            ->getForm()
        ;

        if ($this->getRequest()->getMethod() === 'POST') {
            $form->bindRequest($this->getRequest());
            if ($form->isValid()) {
                $em = $this->getDoctrine()->getEntityManager();

                $em->persist($document);
                $em->flush();

                $this->redirect($this->generateUrl('...'));
            }
        }

        return array('form' => $form->createView());
    }

.. note::

    Gdy tworzysz szablon, nie zapomnij o ustawieniu atrybutu ``enctype``:

    .. code-block:: html+php

        <h1>Upload File</h1>

        <form action="#" method="post" {{ form_enctype(form) }}>
            {{ form_widget(form) }}

            <input type="submit" value="Upload Document" />
        </form>

Poprzedni kontroler automatycznie utworzy encję ``Document`` z wysłaną nazwą,
ale nie zrobi nic z plikiem oraz zmienna ``path`` będzie pusta.

Prostym sposobem na obsłużenie wysłanego pliku jest przeniesienie go zaraz przed
utworzeniem encji oraz ustawienie odpowiedniej wartości zmiennej ``path``.
Rozpocznij od wywołania metody ``upload()`` na obiekcie klasy ``Document``,
którą utworzysz za chwilę::

    if ($form->isValid()) {
        $em = $this->getDoctrine()->getEntityManager();

        $document->upload();

        $em->persist($document);
        $em->flush();

        $this->redirect('...');
    }

Metoda ``upload()`` będzie wykorzystywała możliwości obiektu
:class:`Symfony\\Component\\HttpFoundation\\File\\UploadedFile`,
który zawiera zawartość pola ``file`` po jego wysłaniu::

    public function upload()
    {
        // zmienna file może być pusta jeśli pole nie jest wymagane
        if (null === $this->file) {
            return;
        }

        // używamy oryginalnej nazwy pliku ale nie powinieneś tego robić
        // aby zabezpieczyć się przed ewentualnymi problemami w bezpieczeństwie
        
        // metoda move jako atrybuty przyjmuje ścieżkę docelową gdzie trafi przenoszony plik
        // oraz ścieżkę z której ma przenieś plik
        $this->file->move($this->getUploadRootDir(), $this->file->getClientOriginalName());

        // ustaw zmienną patch ścieżką do zapisanego pliku
        $this->setPath($this->file->getClientOriginalName());

        // wyczyść zmienną file ponieważ już jej nie potrzebujemy
        $this->file = null;
    }

Używanie Cyklu Życia w Callbacks
--------------------------------

Nawet jeśli implementacja działa, to posiada sporą wadę: Co jeśli wystąpią problemy
podczas zapisywania encji? Plik zostanie przeniesiony do swojej końcowej lokalizacji
nawet jeśli zmienna ``path`` nie została zapisana poprawnie.

Aby uniknąć takich problemów, powinieneś zmienić implementację tak aby operację bazowe
oraz przenoszenie pliku stały się bardziej atomowe: jeśli jest problem z zapisem encji
lub też przeniesieniem pliku, wtedy *żadna* z czynności nie powinna zostać wykonana.

Aby to zrobić, powinieneś przenieść plik wtedy gdy Doctrine zapisuje encję do bazy danych.
Można to osiągnąć poprzez podpięcie się pod cykl życia encji::

    /**
     * @ORM\Entity
     * @ORM\HasLifecycleCallbacks
     */
    class Document
    {
    }

Następnie, zrefaktoryzuj klasę ``Document`` tak aby obsłużyć zalety tych wywołań::

    use Symfony\Component\HttpFoundation\File\UploadedFile;

    /**
     * @ORM\Entity
     * @ORM\HasLifecycleCallbacks
     */
    class Document
    {
        /**
         * @ORM\PrePersist()
         * @ORM\PreUpdate()
         */
        public function preUpload()
        {
            if (null !== $this->file) {
                // zrób cokolwiek chcesz aby wygenerować unikalną nazwę
                $this->setPath(uniqid().'.'.$this->file->guessExtension());
            }
        }

        /**
         * @ORM\PostPersist()
         * @ORM\PostUpdate()
         */
        public function upload()
        {
            if (null === $this->file) {
                return;
            }

            // musisz wyrzucać tutaj wyjątek jeśli plik nie może zostać przeniesiony
            // w tym przypadku encja nie zostanie zapisana do bazy
            // metoda move() obiektu UploadedFile robi to automatycznie
            $this->file->move($this->getUploadRootDir(), $this->path);

            unset($this->file);
        }

        /**
         * @ORM\PostRemove()
         */
        public function removeUpload()
        {
            if ($file = $this->getAbsolutePath()) {
                unlink($file);
            }
        }
    }

Klasa robi teraz wszystko czego potrzebujesz: generuje unikalną nazwę pliku przed
zapisem, przenosi plik po zapisie, oraz usuwa plik jeśli encja jest usuwana.

Teraz, kiedy przenoszenie pliku jest obsługiwane przez encję, odwołanie do
``$document->upload()`` powinno zostać usunięte z kontrolera::

    if ($form->isValid()) {
        $em = $this->getDoctrine()->getEntityManager();

        $em->persist($document);
        $em->flush();

        return $this->redirect(...);
    }

.. note::

    Zdarzenia ``@ORM\PrePersist()`` oraz ``@ORM\PostPersist()`` są wywoływane
    przed, oraz po zapisaniu encji do bazy danych. Z drugiej strony, zdarzenia
    ``@ORM\PreUpdate()`` oraz ``@ORM\PostUpdate()`` są wywoływane gdy encja
    jest aktualizowana.

.. caution::

    Metody ``PreUpdate`` oraz ``PostUpdate`` są wywoływane wtedy, i tylko wtedy
    gdy jedno z pól (zmiennych) encji która jest zapisywana uległo zmianie.
    Oznacza to, że domyślnie, jeśli zmienisz tylko wartość ``$file``, zdarzenia
    te nie zostaną wywołane, ponieważ zmienna ta nie jest bezpośrednio zapisywana
    przez Doctrine. Jednym z rozwiązań jest użycie pola ``updated`` które jest
    zapisywane przez Doctrine, i modyfikowana go ręcznie podczas zmiany pliku.

Używanie ``id`` jako nazwy pliku
--------------------------------

Jeśli chcesz użyć ``id`` jako nazwę pliku, implementacja jest nieco inna,
ponieważ musisz zapisać rozszerzenie pliku w zmiennej ``path``,
zamiast aktualnej nazwy::

    use Symfony\Component\HttpFoundation\File\UploadedFile;

    /**
     * @ORM\Entity
     * @ORM\HasLifecycleCallbacks
     */
    class Document
    {
        /**
         * @ORM\PrePersist()
         * @ORM\PreUpdate()
         */
        public function preUpload()
        {
            if (null !== $this->file) {
                $this->setPath($this->file->guessExtension());
            }
        }

        /**
         * @ORM\PostPersist()
         * @ORM\PostUpdate()
         */
        public function upload()
        {
            if (null === $this->file) {
                return;
            }

            // musisz wyrzucać tutaj wyjątek jeśli plik nie może zostać przeniesiony
            // w tym przypadku encja nie zostanie zapisana do bazy
            // metoda move() obiektu UploadedFile robi to automatycznie
            $this->file->move($this->getUploadRootDir(), $this->id.'.'.$this->file->guessExtension());

            unset($this->file);
        }

        /**
         * @ORM\PostRemove()
         */
        public function removeUpload()
        {
            if ($file = $this->getAbsolutePath()) {
                unlink($file);
            }
        }

        public function getAbsolutePath()
        {
            return null === $this->path ? null : $this->getUploadRootDir().'/'.$this->id.'.'.$this->path;
        }
    }

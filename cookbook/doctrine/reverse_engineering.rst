.. index::
   single: Doctrine; Generating entities from existing database

Jak wygenerować Encje z Istniejącej Bazy danych
===============================================

Kiedy zaczynasz pracować nad nowym projektem który używa bazy danych,
dwie różne sytuacje wychodzą naturalnie. W większości przypadków,
model bazy danych jest zaprojektowany i zbudowany od podstaw.
Czasami jednak zaczniesz pracę z istniejącym i niezmienialnym modelem bazy danych.
Na szczęście, Doctrine posiada kilka narzędzi które pomogą wygenerować modele
klas z istniejącej już bazy danych.

.. note::

    Tak jak mówi `Doctrine tools documentation`_, inżynieria odwrotna (reverse engineering)
    jest jednorazowym procesem do rozpoczęcia pracy z projektem. Doctrine jest wstanie 
    zinterpretować około 70-80% z ważnych informacji do zmapowania bazując na polach,
    indeksach oraz kluczach obcych. Doctrine nie poradzi sobie z takimi zagadnieniami 
    jak odwrotne połączenie (inverse associations), dziedziczenie (inheritance),
    encjami których klucze obce są kluczami głównymi lub semantycznych operacjach
    na połączeniach takich jak kaskady lub cykle wydarzeń. Będzie potrzebna pewna 
    dodatkowa praca aby dopasować specyficzne wymagania modelu bazy do wygenerowanych 
    encji.

Ten poradnik zakłada że używasz prostej aplikacji bloga z następującymi tabelami:
``blog_post`` i ``blog_comment``. Komentarz jest powiązany z postem poprzez klucz obcy.


    CREATE TABLE `blog_post` (
      `id` bigint(20) NOT NULL AUTO_INCREMENT,
      `title` varchar(100) COLLATE utf8_unicode_ci NOT NULL,
      `content` longtext COLLATE utf8_unicode_ci NOT NULL,
      `created_at` datetime NOT NULL,
      PRIMARY KEY (`id`),
    ) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

    CREATE TABLE `blog_comment` (
      `id` bigint(20) NOT NULL AUTO_INCREMENT,
      `post_id` bigint(20) NOT NULL,
      `author` varchar(20) COLLATE utf8_unicode_ci NOT NULL,
      `content` longtext COLLATE utf8_unicode_ci NOT NULL,
      `created_at` datetime NOT NULL,
      PRIMARY KEY (`id`),
      KEY `blog_comment_post_id_idx` (`post_id`),
      CONSTRAINT `blog_post_id` FOREIGN KEY (`post_id`) REFERENCES `blog_post` (`id`) ON DELETE CASCADE
    ) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

Zanim zagłębimy się w zadanie, upewnij się czy masz odpowiednio skonfigurowaną bazę danych
w pliku ``app/config/parameters.ini`` (lub gdziekolwiek trzymasz konfigurację bazy) 
oraz że masz bundle do obsługiwania swoich klas encji. W tym poradniku, zakładamy że
``AcmeBlogBundle`` istnieje oraz że znajduje się w katalogu ``src/Acme/BlogBundle``.

Pierwszym krokiem do zbudowania klas encji z istniejącej bazy danych jest zapytanie Doctrine
o zanalizowanie bazy oraz wygenerowaniu odpowiednich plików metadata.
Pliki metadata opisują klasę encji do wygenerowania bazując na polach tabelii.

.. code-block:: bash

    php app/console doctrine:mapping:convert xml ./src/Acme/BlogBundle/Resources/config/doctrine/metadata/orm --from-database --force

Te polecenie lini komend wywołuję analizę Doctrine na bazie oraz generuje pliki XML metadata w katalogu 
``src/Acme/BlogBundle/Resources/config/doctrine/metadata/orm`` Twojego bundla.

.. tip::

    Możliwe jest także wygenerowanie klas metadata w formacie YAML, poprzez zmianę pierwszego argumentu na `yml`.

Wygenerowany plik metadata ``BlogPost.dcm.xml`` wygląda następująco:

.. code-block:: xml

    <?xml version="1.0" encoding="utf-8"?>
    <doctrine-mapping>
      <entity name="BlogPost" table="blog_post">
        <change-tracking-policy>DEFERRED_IMPLICIT</change-tracking-policy>
        <id name="id" type="bigint" column="id">
          <generator strategy="IDENTITY"/>
        </id>
        <field name="title" type="string" column="title" length="100"/>
        <field name="content" type="text" column="content"/>
        <field name="isPublished" type="boolean" column="is_published"/>
        <field name="createdAt" type="datetime" column="created_at"/>
        <field name="updatedAt" type="datetime" column="updated_at"/>
        <field name="slug" type="string" column="slug" length="255"/>
        <lifecycle-callbacks/>
      </entity>
    </doctrine-mapping>

Gdy pliki metadata są już wygenerowane, możesz wywołać w Doctrine import 
schemy oraz budowanie powiązanych klas encji poprzez wykonanie poniższych poleceń.

.. code-block:: bash

    php app/console doctrine:mapping:import AcmeBlogBundle annotation
    php app/console doctrine:generate:entities AcmeBlogBundle

Pierwsze polecenie generuje klasy encji z mapowaniem notacjami,
ale oczywiście możesz zmienić argument ``annotation`` na ``xml`` lub ``yml``.
Nowo utworzona klasa encji ``BlogComment`` wygląda następująco:

.. code-block:: php

    <?php

    // src/Acme/BlogBundle/Entity/BlogComment.php
    namespace Acme\BlogBundle\Entity;

    use Doctrine\ORM\Mapping as ORM;

    /**
     * Acme\BlogBundle\Entity\BlogComment
     *
     * @ORM\Table(name="blog_comment")
     * @ORM\Entity
     */
    class BlogComment
    {
        /**
         * @var bigint $id
         *
         * @ORM\Column(name="id", type="bigint", nullable=false)
         * @ORM\Id
         * @ORM\GeneratedValue(strategy="IDENTITY")
         */
        private $id;

        /**
         * @var string $author
         *
         * @ORM\Column(name="author", type="string", length=100, nullable=false)
         */
        private $author;

        /**
         * @var text $content
         *
         * @ORM\Column(name="content", type="text", nullable=false)
         */
        private $content;

        /**
         * @var datetime $createdAt
         *
         * @ORM\Column(name="created_at", type="datetime", nullable=false)
         */
        private $createdAt;

        /**
         * @var BlogPost
         *
         * @ORM\ManyToOne(targetEntity="BlogPost")
         * @ORM\JoinColumn(name="post_id", referencedColumnName="id")
         */
        private $post;
    } 

Jak widzisz, Doctrine przekonwertował wszystkie pola tabel do prywatnych zmiennych klasy 
wraz z notacjami. Najbardziej imponujące jest to że odkrył powiązanie z klasą encji ``BlogPost``
bazując na kluczu obcym.
W związku z tym, możesz odnaleść prywatną zmienną ``$post`` zmapowaną z encją ``BlogPost``
w klasie encji ``BlogComment``.

Ostatnie polecenie generuje wszystkie gettery oraz settery dla właściwości dwóch klas encji ``BlogPost``
oraz ``BlogComment``. Wygenerowane encje są gotowe do użycia. Miłej zabawy!

.. _`Doctrine tools documentation`: http://www.doctrine-project.org/docs/orm/2.0/en/reference/tools.html#reverse-engineering

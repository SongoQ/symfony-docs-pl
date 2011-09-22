.. index::
   single: Forms; Fields; file

file Typ Pola
=============

Typ ``file`` reprezentuje pole do wysyłania pliku w formularzu.

+------------------+---------------------------------------------------------------------+
| Renderowane jako | ``input`` ``file`` pole                                             |
+------------------+---------------------------------------------------------------------+
| Odziedziczone    | - `required`_                                                       |
| opcje            | - `label`_                                                          |
|                  | - `read_only`_                                                      |
|                  | - `error_bubbling`_                                                 |
+------------------+---------------------------------------------------------------------+
| Rodzic           | :doc:`form</reference/forms/types/field>`                           |
+------------------+---------------------------------------------------------------------+
| Klasa            | :class:`Symfony\\Component\\Form\\Extension\\Core\\Type\\FileType`  |
+------------------+---------------------------------------------------------------------+

Podstawowe Użycie
-----------------

Wyobraźmy sobie że posiadasz taką definicję formularza:

.. code-block:: php

    $builder->add('attachment', 'file');

.. caution::

    Nie zapomnij dodać atrybutu ``enctype`` w tagu form: ``<form
    action="#" method="post" {{ form_enctype(form) }}>``.

Gdy formularz zostanie wysłany, pole ``attachment`` stanie się instancją
:class:`Symfony\\Component\\HttpFoundation\\File\\UploadedFile`. Może zostać
użyty do przeniesienia pliku ``attachment`` do stałej lokalizacji:

.. code-block:: php

    use Symfony\Component\HttpFoundation\File\UploadedFile;

    public function uploadAction()
    {
        // ...

        if ($form->isValid()) {
            $someNewFilename = ...
        
            $form['attachment']->getData()->move($dir, $someNewFilename);

            // ...
        }

        // ...
    }

Metoda ``move()`` pobiera jako atrybuty folder docelowy oraz nazwę pliku.
Masz kilka możliwości aby ustawić nazwę pliku::

    // użyj oryginalnej nazwy
    $file->move($dir, $file->getClientOriginalName());

    // wygeneruj losowa nazwę i postaraj się odgadnąć rozszerzenie (bardziej bezpieczne)
    $extension = $file->guessExtension();
    if (!$extension) {
        // rozszerzenie nie może zostać znalezione
        $extension = 'bin';
    }
    $file->move($dir, rand(1, 99999).'.'.$extension);

Używanie oryginalnej nazwy poprzez ``getClientOriginalName()`` nie jest bezpieczne
ponieważ nazwą mógł manipulować użytkownik końcowy. Ponadto, może zawierać znaki nie dozwolone
w nazwach plików. Należy wyczyścić nazwę przed jej bezpośrednim użyciem.

Przeczytaj :doc:`cookbook </cookbook/doctrine/file_uploads>` aby zobaczyć jak zarządzać
wysyłanymi plikami w połączeniu z encjami Doctrine.

Odziedziczone opcje
-------------------

Opcje te są odziedziczone z typu :doc:`field</reference/forms/types/field>`:

.. include:: /reference/forms/types/options/required.rst.inc

.. include:: /reference/forms/types/options/label.rst.inc

.. include:: /reference/forms/types/options/read_only.rst.inc

.. include:: /reference/forms/types/options/error_bubbling.rst.inc

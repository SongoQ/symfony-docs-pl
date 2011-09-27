.. index::
   single: Forms; Fields; time

time Typ Pola
=============

Pole do przechwytywania czasu.

Pole te może zostać wyrenderowane jako pole tekstowe, lub lista pól tekstowych (np. godzina, minuta,
sekunda) lub lista pól wyboru. Przekazane dane mogą być w formacie obiektu ``DateTime``, ciąg
znaków, timestamp lub tablica.

+------------------------+------------------------------------------------------------------------------+
| Podstawowe Typy Danych | może być ``DateTime``, string, timestamp, lub array (zobacz opcję ``input``) |
+------------------------+------------------------------------------------------------------------------+
| Renderowane jako       | może być generowany na kilka sposobów (zobacz poniżej)                       |
+------------------------+------------------------------------------------------------------------------+
| Opcje                  | - `widget`_                                                                  |
|                        | - `input`_                                                                   |
|                        | - `with_seconds`_                                                            |
|                        | - `hours`_                                                                   |
|                        | - `minutes`_                                                                 |
|                        | - `seconds`_                                                                 |
|                        | - `data_timezone`_                                                           |
|                        | - `user_timezone`_                                                           |
+------------------------+------------------------------------------------------------------------------+
| Rodzic                 | form                                                                         |
+------------------------+------------------------------------------------------------------------------+
| Klasa                  | :class:`Symfony\\Component\\Form\\Extension\\Core\\Type\\TimeType`           |
+------------------------+------------------------------------------------------------------------------+

Podstawowe Użycie
-----------------

Ten typ pola jest wysoce konfigurowalny, lecz prosty w użyciu. Najważniejszymi opcjami są
``input`` oraz ``widget``.

Przypuścmy że masz pole ``startTime`` którego danymi wejściowymi jest obiekt ``DateTime``. Poniższy kod 
konfiguruje typ ``time`` dla tego pola jako trzy różne pola wyboru:

.. code-block:: php

    $builder->add('startTime', 'time', array(
        'input'  => 'datetime',
        'widget' => 'choice',
    ));

Opcja ``input`` musi zostać zmieniona w stosunku do dostarczonego typu danych. 
Dla przykładu, jeśli danymi pola ``startTime`` jest unixowy timestamp, musisz 
zamienić wartość opcji ``input`` na ``timestamp``:

.. code-block:: php

    $builder->add('startTime', 'time', array(
        'input'  => 'timestamp',
        'widget' => 'choice',
    ));

Pole wspiera wartości ``array`` oraz ``string`` dla wartości opcji ``input``.

Opcje Pola
----------

widget
~~~~~~

**typ**: ``string`` **domyślnie**: ``choice``

Podstawowym sposobem, w jakim pole powinno być renderowane może być jedną z następujących opcji:

* ``choice``: renderuje dwa (lub trzy jeśli opcja `with_seconds`_ jest ustawiona na true) pola wyboru.

* ``text``: renderuje dwa lub trzy pola tekstowe (godzina, minuta, sekunda).

* ``single_text``: renderuje pojedyńcze pole tekstowe. Dane wprowadzone przez użytkownika będą walidowane
  pod kontem wzorca ``hh:mm`` (lub ``hh:mm:ss`` jeśli używane są sekundy).

input
~~~~~

**typ**: ``string`` **domyślnie**: ``datetime``

Format ``wejściowy`` danych - czyli format, w których data jest przechowywana w Twoim obiekcie. 
Prawidłowe wartości to:

* ``string`` (np. ``12:17:26``)
* ``datetime`` (obiekt ``DateTime``)
* ``array`` (e.g. ``array('hour' => 12, 'minute' => 17, 'second' => 26)``)
* ``timestamp`` (e.g. ``1307232000``)

Wartość wysyłana w formularzu będzie normalizowana do tego formatu.

.. include:: /reference/forms/types/options/with_seconds.rst.inc

.. include:: /reference/forms/types/options/hours.rst.inc

.. include:: /reference/forms/types/options/minutes.rst.inc

.. include:: /reference/forms/types/options/seconds.rst.inc

.. include:: /reference/forms/types/options/data_timezone.rst.inc

.. include:: /reference/forms/types/options/user_timezone.rst.inc

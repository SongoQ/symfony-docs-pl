.. index::
   single: konfiguracja; WebProfiler

Konfiguracja WebProfilerBundle
==============================

Pełna domyślna konfiguracja
---------------------------

.. configuration-block::

    .. code-block:: yaml
       :linenos:

        web_profiler:

            # PRZESTARZAŁE, it is not useful anymore and can be removed safely from your configuration
            verbose:              true

            # display the web debug toolbar at the bottom of pages with a summary of profiler info
            toolbar:              false
            position:             bottom

            # gives you the opportunity to look at the collected data before following the redirect
            intercept_redirects: false

    .. code-block:: xml
       :linenos:

        <web-profiler:config
            toolbar="false"
            verbose="true"
            intercept_redirects="false"
        />
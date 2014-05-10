.. index::
    single: Multilanguage; DoctrinePHPCRBundle

Doctrine PHPCR-ODM Multilanguage Support
========================================

To use the multilanguage features of PHPCR-ODM you need to enable locales in
the configuration.

Translation Configuration
-------------------------

To use translated documents, you need to configure the available languages:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        doctrine_phpcr:
            odm:
                # ...
                locales:
                    en: [de, fr]
                    de: [en, fr]
                    fr: [en, de]
                locale_fallback: hardcoded

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services">

            <config xmlns="http://doctrine-project.org/schema/symfony-dic/odm/phpcr">

                <odm locale-fallback="hardcoded">
                    <!-- ... -->
                    <locale name="en">
                        <fallback>de</fallback>
                        <fallback>fr</fallback>
                    </locale>

                    <locale name="de">
                        <fallback>en</fallback>
                        <fallback>fr</fallback>
                    </locale>

                    <locale name="fr">
                        <fallback>en</fallback>
                        <fallback>de</fallback>
                    </locale>
                </odm>
            </config>
        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('doctrine_phpcr', array(
            'odm' => array(
                // ...
                'locales' => array(
                    'en' => array('de', 'fr'),
                    'de' => array('en', 'fr'),
                    'fr' => array('en', 'de'),
                ),
                'locale_fallback' => 'hardcoded',
            )
        );

The ``locales`` is a list of alternative locales to look up if a document
is not translated to the requested locale.

This bundle provides a request listener that gets activated when any locales
are configured. This listener updates PHPCR-ODM to use the locale Symfony
determined for this request, if that locale is in the list of keys defined
under ``locales``.

Fallback strategies
~~~~~~~~~~~~~~~~~~~

There are several strategies to adjust the fallback order for the selected
locale based on the accepted languages of the request (determined by Symfony
from the ``Accept-Language`` HTML header). All of them will never add any
locales that where not configured in the ``locales`` to avoid a request
injecting unexpected things into your repository:

* ``hardcoded``: The default strategy does not update the fallback order from
  the request;
* ``replace``: takes the accepted locales from the request and updates the
  fallback order with them, removing any locales not found in the request;
* ``merge``: does the same as ``replace`` but then adds locales not found in
  the request but on the ``locales`` configuration back to the end of the
  fallback list. This reorders the locales without losing any of them.

Translated documents
--------------------

To make a document translated, you need to define the ``translator`` attribute
on the document mapping, and you need to map the ``locale`` field. Then you can
use the ``translated`` attribute on all fields that should be different
depending on the locale.

.. configuration-block::

    .. code-block:: php

        <?php

        use Doctrine\ODM\PHPCR\Mapping\Annotations as PHPCR;

        /**
         * @PHPCR\Document(translator="attribute")
         */
        class MyPersistentClass
        {
            /**
             * The language this document currently is in
             * @PHPCR\Locale
             */
            private $locale;

            /**
             * Untranslated property
             * @PHPCR\Date
             */
            private $publishDate;

            /**
             * Translated property
             * @PHPCR\String(translated=true)
             */
            private $topic;

            /**
             * Language specific image
             * @PHPCR\Binary(translated=true)
             */
            private $image;
        }

    .. code-block:: xml

        <doctrine-mapping>
            <document class="MyPersistentClass"
                      translator="attribute">
                <locale fieldName="locale" />
                <field fieldName="publishDate" type="date" />
                <field fieldName="topic" type="string" translated="true" />
                <field fieldName="image" type="binary" translated="true" />
            </document>
        </doctrine-mapping>

    .. code-block:: yaml

        MyPersistentClass:
          translator: attribute
          locale: locale
          fields:
            publishDate:
                type: date
            topic:
                type: string
                translated: true
            image:
                type: binary
                translated: true

Unless you explicitly interact with the multilanguage features of PHPCR-ODM,
documents are loaded in the request locale and saved in the locale they where
loaded. (This could be a different locale, if the PHPCR-ODM did not find the
requested locale and had to fall back to an alternative locale.)

.. tip::

    For more information on multilingual documents, see the
    `PHPCR-ODM documentation on Multilanguage`_.

.. _`PHPCR-ODM documentation on Multilanguage`: http://docs.doctrine-project.org/projects/doctrine-phpcr-odm/en/latest/reference/multilang.html

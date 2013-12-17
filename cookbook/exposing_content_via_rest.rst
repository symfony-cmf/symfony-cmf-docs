.. index::
    single: Tutorial, REST
    single: ContentBundle

Exposing Content via REST
=========================

Many applications need to expose content via REST APIs to partners or to
enable integration into other applications. As the CMF is build on top
of Symfony2, its possible to leverage many of the available Bundles to
provide a REST API for content stored in the CMF. This cookbook will
detail how to provide a read API using the following Bundles:

* :doc:`../bundles/content/index`;
* `FOSRestBundle`_.
* `JMSSerializerBundle`_.

It is assumed that you have:

* A working knowledge of the Symfony2 CMF framework;
* An application with the ContentBundle setup.

Installation
------------

The ContentBundle provides support for the `FOSRestBundle view layer`_
which is automatically activated by the ``ContentController`` when the
Bundle is available. Furthermore, the FOSRestBundle needs a serializer
to generate the REST output. The best choice is the JMSSerializerBundle:

.. code-block:: javascript

    {
        ..
        "require": {
            ..
            "friendsofsymfony/rest-bundle": "1.0.*",
            "jms/serializer-bundle": "0.13.*",
        }
    }

.. note::

    Both Bundles are already required by the CreateBundle.

.. note::

    When using PHPCR-ODM it is necessary to require at least version 1.0.1
    of ``doctrine\phpcr-odm``.

Then use composer to update your projects vendors:

.. code-block:: bash

    php composer.phar update

Configuring FOSRestBundle
-------------------------

Here is an example configuration for the FOSRestBundle.

.. code-block:: jinja

    fos_rest:
        # configure the view handler
        view:
            force_redirects:
              html: true
            formats:
                json: true
                xml: true
            templating_formats:
                html: true
        # add a content negotiation rule, enabling support for json/xml for the entire website
        format_listener:
            rules:
                - { path: ^/, priorities: [ html, json, xml ], fallback_format: html, prefer_extension: false }

Using the REST API
------------------

This all it takes to enable read support via JSON or XML.

.. code-block:: bash

    curl http://my-cmf.org/app_dev.php -H Accept:application/json
    curl http://my-cmf.org/app_dev.php -H Accept:application/xml
    curl http://my-cmf.org/app_dev.php -H Accept:text/html

The JMS serializer comes with sense defaults for Doctrine object mappers.
However it might be necessary to add additional mapping to more tightly
control what gets exposed. See the `documentation of the JMS serializer`_
for details.

.. note::

    The `default response format changed between 1.0 and 1.1 of the ContentBundle`_. In
    1.0 the response is wrapped inside an array/tag. This is no longer the case in 1.1

.. _`FOSRestBundle`: https://github.com/FriendsOfSymfony/FOSRestBundle
.. _`JMSSerializerBundle`: https://github.com/schmittjoh/JMSSerializerBundle
.. _`FOSRestBundle view layer`: https://github.com/FriendsOfSymfony/FOSRestBundle/blob/master/Resources/doc/2-the-view-layer.md
.. _`documentation of the JMS serializer`: http://jmsyst.com/libs/#serializer
.. _`default response format changed between 1.0 and 1.1 of the ContentBundle`: https://github.com/symfony-cmf/ContentBundle/pull/91

.. index::
    single: Tutorial, REST
    single: ContentBundle

Exposing Content via REST
=========================

Many applications need to expose content via REST APIs to partners or to
enable integration into other applications. As the CMF is build on top
of Symfony2, it's possible to leverage many of the available bundles to
provide a REST API for content stored in the CMF. This cookbook will
detail how to provide a read API using the following bundles:

* :doc:`Cmf ContentBundle <../bundles/content/index>`;
* `FOSRestBundle`_;
* `JMSSerializerBundle`_.

It is assumed that you have:

* A working knowledge of the Symfony2 CMF framework;
* An application with the ContentBundle setup.

Installation
------------

The ContentBundle provides support for the `FOSRestBundle view layer`_,
which is automatically activated by the ``ContentController`` when the
bundle is available. Furthermore, the FOSRestBundle needs a serializer
to generate the REST output. The best choice is the JMSSerializerBundle:

.. code-block:: javascript

    {
        ..
        "require": {
            ..
            "friendsofsymfony/rest-bundle": "1.*",
            "jms/serializer-bundle": "0.13.*",
        }
    }

.. note::

    Both bundles are already required by the CreateBundle.

.. caution::

    When using PHPCR-ODM it is necessary to require at least version 1.0.1
    of ``doctrine\phpcr-odm``.

Then use Composer_ to update your projects vendors:

.. code-block:: bash

    $ php composer.phar update

Configuring FOSRestBundle
-------------------------

Here is an example configuration for the FOSRestBundle.

.. configuration-block::

    .. code-block:: yaml

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

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services">

            <config xmlns="http://example.org/schema/dic/fos_rest">
                <!-- configure the view handler -->
                <view>
                    <force-redirect name="html">true</force-redirect>

                    <format name="json">true</format>
                    <format name="xml">true</format>

                    <templating-format name="html">true</templating-format>
                </view>

                <!-- add a content negotiation rule, enabling support for json/xml for the entire website -->
                <format-listener>
                    <rule path="^/"
                        fallback-format="html"
                        prefer-extension="false"
                        priorities="html,json,xml"
                    />
                </format-listener>
            </config>
        </container>

    .. code-block:: php

        $container->loadFromExtension('fos_rest', array(
            // configure the view handler
            'view' => array(
                'force_redirects' => array(
                    'html' => true,
                ),
                'formats' => array(
                    'json' => true,
                    'xml' => true,
                ),
                'templating_formats' => array(
                    'html' => true,
                ),
            ),
            // add a content negotiation rule, enabling support for json/xml for the entire website
            'format_listener' => array(
                'rules' => array(
                    array(
                        'path' => '^/',
                        'priorities' => array('html', 'json', 'xml'),
                        'fallback_format' => 'html',
                        'prefer_extension' => false,
                    ),
                ),
            ),
        ));

Using the REST API
------------------

After you configured the FOSRestBundle, you need to execute the following
commands:

.. code-block:: bash

    curl http://my-cmf.org/app_dev.php -H Accept:application/json
    curl http://my-cmf.org/app_dev.php -H Accept:application/xml
    curl http://my-cmf.org/app_dev.php -H Accept:text/html

This is all it takes to enable read support via JSON or XML!

The JMS serializer comes with sense defaults for Doctrine object mappers.
However it might be necessary to add additional mapping to more tightly
control what gets exposed. See the `documentation of the JMS serializer`_
for details.

.. versionadded:: 1.1
    The `default response format changed between 1.0 and 1.1 of the ContentBundle`_.
    In 1.0 the response is wrapped inside an array/tag. This is no longer the
    case in 1.1

.. _`FOSRestBundle`: https://github.com/FriendsOfSymfony/FOSRestBundle
.. _`JMSSerializerBundle`: https://github.com/schmittjoh/JMSSerializerBundle
.. _`FOSRestBundle view layer`: https://github.com/FriendsOfSymfony/FOSRestBundle/blob/master/Resources/doc/2-the-view-layer.md
.. _Composer: http://getcomposer.org/
.. _`documentation of the JMS serializer`: http://jmsyst.com/libs/#serializer
.. _`default response format changed between 1.0 and 1.1 of the ContentBundle`: https://github.com/symfony-cmf/ContentBundle/pull/91

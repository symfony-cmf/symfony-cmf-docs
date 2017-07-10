.. index::
    single: ContentBundle; Rest

Exposing Content via REST
=========================

Many applications need to expose content via REST APIs to partners or to
enable integration into other applications. As the CMF is build on top
of Symfony, it's possible to leverage many of the available bundles to
provide a REST API for content stored in the CMF. This guide will
detail how to provide a read API for your content using the
`FOSRestBundle`_ and the `JMSSerializerBundle`_.

Installation
------------

The ContentBundle provides support for the `FOSRestBundle view layer`_,
which is automatically activated by the ``ContentController`` when the
bundle is available. Furthermore, the FOSRestBundle needs a serializer
to generate the REST output. The best choice is the JMSSerializerBundle:

.. code-block:: javascript

    {
        ...
        "require": {
            ...
            "friendsofsymfony/rest-bundle": "^1.7",
            "jms/serializer-bundle": "^1.1"
        }
    }

And instantiate the two bundles in your application kernel.

Then use Composer_ to update your projects vendors:

.. code-block:: bash

    $ composer update

Configuring FOSRestBundle
-------------------------

Here is an example configuration for the FOSRestBundle.

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
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

        <!-- app/config/config.xml -->
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

        // app/config/config.php
        $container->loadFromExtension('fos_rest', [
            // configure the view handler
            'view' => [
                'force_redirects' => [
                    'html' => true,
                ],
                'formats' => [
                    'json' => true,
                    'xml' => true,
                ],
                'templating_formats' => [
                    'html' => true,
                ],
            ],
            // add a content negotiation rule, enabling support for json/xml for the entire website
            'format_listener' => [
                'rules' => [
                    [
                        'path' => '^/',
                        'priorities' => ['html', 'json', 'xml'],
                        'fallback_format' => 'html',
                        'prefer_extension' => false,
                    ],
                ],
            ],
        ]);

Using the REST API
------------------

This is all it takes to enable read support via JSON or XML!
Test if the setup works as expected with curl:

.. code-block:: bash

    curl http://my-cmf.org/app_dev.php -H Accept:application/json
    curl http://my-cmf.org/app_dev.php -H Accept:application/xml
    curl http://my-cmf.org/app_dev.php -H Accept:text/html


The JMS serializer comes with sensible defaults for Doctrine object mappers.
However it might be necessary to add additional mapping to more tightly
control what gets exposed. See the `documentation of the JMS serializer`_
for details.

.. versionadded:: 1.1
    The `default response format changed between 1.0 and 1.1 of the ContentBundle`_.
    In 1.0 the response is wrapped inside an array/tag. This is no longer the
    case in 1.1

.. _`FOSRestBundle`: https://github.com/FriendsOfSymfony/FOSRestBundle
.. _`JMSSerializerBundle`: https://github.com/schmittjoh/JMSSerializerBundle
.. _`FOSRestBundle view layer`: https://symfony.com/doc/master/bundles/FOSRestBundle/2-the-view-layer.html
.. _Composer: https://getcomposer.org/
.. _`documentation of the JMS serializer`: http://jmsyst.com/libs/#serializer
.. _`default response format changed between 1.0 and 1.1 of the ContentBundle`: https://github.com/symfony-cmf/content-bundle/pull/91

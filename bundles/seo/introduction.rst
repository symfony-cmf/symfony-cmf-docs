SeoBundle
=========

    This bundle provides a layer on top of the `SonataSeoBundle`_, to make it
    easier to collect SEO data from content documents.

Installation
------------

You can install this bundle `with composer`_ using the
``symfony-cmf/seo-bundle`` package on `Packagist`_.

This bundle extends the SonataSeoBundle which must be registered in the
kernel as well::

    // app/appKernel.php

    // ...
    public function registerBundles()
    {
        $bundles = [
            // ...
            new Sonata\SeoBundle\SonataSeoBundle(),
            new Symfony\Cmf\Bundle\SeoBundle\CmfSeoBundle(),
        )];

        // ...

        return $bundles;
    }

Usage
~~~~~

SEO data tracks some or all of the following:

* The title;
* The meta keywords;
* The meta description;
* The original URL (when more than one URL contains the same content).
* Anything else that uses the ``<meta>`` tag with the ``property``, ``name``
  or ``http-equiv`` type (e.g. Open Graph data).

The simplest use of this bundle would be to just set some configuration to the
``sonata_seo`` configuration section and use the twig helper in your templates.

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        sonata_seo:
            page:
                title: Page's default title
                metas:
                    name:
                        description: The default description of the page
                        keywords: default, sonata, seo

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('sonata_seo', [
            'page' => [
                'title' => 'Page's default title',
                'metas' => [
                    'name' => [
                        'description' => 'default description',
                        'keywords' => 'default, key, other',
                    ],
                ],
            ],
        ]);

This sets default values for the ``SeoPage`` value object. You can later update
that object with more precise information. It is available as service
``sonata.seo.page.default``.

To render the information, use the following twig functions in your templates:

.. code-block:: html+jinja

    <!-- app/Resources/views/base.html.twig -->
    <!DOCTYPE html>
    <html>
        <head>
            {{ sonata_seo_title() }}

            {{ sonata_seo_metadatas() }}
        </head>
        <body>
            <p>Some page body.</p>
        </body>
    </html>

This will render the last title set on the ``SeoPage`` ("Page's default title"
if you did not add calls to the value object in your code). The information
added for description and keywords will render as ``<meta>`` HTML tags.

.. seealso::

    To get a deeper look into the SonataSeoBundle, you should visit the
    `Sonata documentation`_.

Using the CmfSeoBundle
----------------------

The basic example shown above works perfectly without the CmfSeoBundle. The
CmfSeoBundle provides extension points to extract the SEO data from
content documents, e.g. a ``StaticContent``, along with utility systems
to automatically extract the information.

The process is:

1. The content listener checks for a document in the request
2. It invokes ``SeoPresentationInterface::updateSeoPage``
3. The presentation checks of the document provides a ``SeoMetadata`` value
   object and runs the metadata extractors.
4. The presentation updates the Sonata ``SeoPage`` with the gathered meta data.

.. _bundles-seo-content-listener:

The ContentListener
~~~~~~~~~~~~~~~~~~~

The ``Symfony\Cmf\Bundle\SeoBundle\EventListener\ContentListener`` looks for a
content document in the request attributes. If the listener finds a document,
it calls ``SeoPresentationInterface::updateSeoPage`` to populate the metadata
information if possible.

If the :doc:`RoutingBundle <../routing/introduction>` is installed, the default
attribute name is defined by the constant ``DynamicRouter::CONTENT_KEY``. When
not using the RoutingBundle, you need to disable the listener or configure a
key in ``cmf_seo.content_key``.

If you need different behavior, build your own mechanism to extract the SEO
information and disable this listener at ``cmf_seo.content_listener.enabled``
to avoid unnecessary code execution. Things that happen on every request are
critical for your application performance.

The Twig Extension
~~~~~~~~~~~~~~~~~~

.. versionadded:: 1.2
    The twig extension was added in SeoBundle 1.2.

This bundle provides a twig function ``cmf_seo_update_metadata``
which lets you populate the seo page from an object.
For details on using the twig extension, read :doc:`twig`.

Extracting Metadata
~~~~~~~~~~~~~~~~~~~

A service implementing ``SeoPresentationInterface`` is responsible for
determining metadata from an object and updating the Sonata ``SeoPage`` with that
information. A default implementation is provided as ``cmf_seo.presentation``.

Defining Metadata
~~~~~~~~~~~~~~~~~

This bundle provides two ways to define metadata on objects:

#. Implementing the ``SeoAwareInterface`` and persisting the ``SeoMetadata``
   with the object.
#. Using ``ExtractorInterface`` instances, to extract the ``SeoMetadata`` from
   already existing values (e.g. the title of the page).

You can also combine both ways, even on the same document. In that case, the
persisted ``SeoMetadata`` can be changed by the extractors, to add or tweak
the current available SEO information. For instance, if you are writing a
``BlogPost`` class, you want the SEO keywords to be set to the tags/category
of the post and any additional tags set by the editor.

Persisting the ``SeoMetadata`` with the document makes it easy to override SEO
information for the editor, while using the extractors adds the convenience
that values from the normal content of the document can be reused.

Both methods are documented in detail in separate sections:

* :doc:`seo_aware`
* :doc:`extractors`

Choosing the Original Route Pattern
-----------------------------------

Search engines don't like it when you provide the same content under several
URLs. The CMF allows you to have several URLs for the same content if you need
that. There are two solutions to avoid penalties with search engines:

* Create a canonical link that identifies the original URL:
  ``<link rel="canonical" href="/route/org/content">``
* Define an "original url" and redirect all duplicate URLs to it.

The ``SeoMetadata`` can be configured with the original URL for the current
page. By default, this bundle will create a canonical link for the page. If
you want to change that to redirect instead, you can set the
``original_route_pattern`` option:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        cmf_seo:
            original_route_pattern: redirect

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="utf-8"?>
        <config xmlns="http://cmf.symfony.com/schema/dic/seo"
            original-route-pattern="redirect"
        />

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension(
            'cmf_seo' => [
                'original_route_pattern' => 'redirect',
            ],
        );

.. _bundles-seo-title-description-template:

Defining a Title and Description Template
-----------------------------------------

Most of the times, the title of a site has a static and a dynamic part. For
instance, "The title of the Page - Symfony". Here, "- Symfony" is static and
"The title of the Page" will be replaced by the current title. It would not be
nice if you had to add this static part to all your titles in documents.

The CmfSeoBundle allows you to define a title and description template for
this reason. When using these settings, there are 2 placeholders available:
``%content_title%`` and ``%content_description%``. These will be replaced with
the title extracted from the content object and the description extracted from
the content object.

.. caution::

    The title and description template is only used when the title is not set
    on the content object or when the content object is not available,
    otherwise it'll use the default set by the SonataSeoBundle. You should
    make sure that the defaults also follow the template.

For instance, to configure the titles of the symfony.com pages, you would do:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        cmf_seo:
            title: "%%content_title%% - Symfony"

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="utf-8"?>
        <config xmlns="http://cmf.symfony.com/schema/dic/seo"
            title="%%content_title%% - Symfony"
        />

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('cmf_seo', [
            'title' => '%%content_title%% - Symfony',
        ]);

.. caution::

    Be sure to escape the percentage characters by using a double percentage
    character, otherwise the container will try to replace it with the value
    of a container parameter.

This syntax might look familiar if you have used the Translation component
before. And that's correct, under the hood the Translation component is used
to replace the placeholders with the correct values. This also means you get
Multi Language Support for free!

For instance, you can do:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        cmf_seo:
            title: seo.title
            description: seo.description

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="utf-8"?>
        <config xmlns="http://cmf.symfony.com/schema/dic/seo"
            title="seo.title"
            description="seo.description"
        />

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('cmf_seo', [
            'title' => 'seo.title',
            'description' => 'seo.description',
        ]);

And then configure the translation messages:

.. configuration-block::

    .. code-block:: xml

        <!-- app/Resources/translations/messages.en.xliff -->
        <?xml version="1.0" encoding="utf-8"?>
        <xliff xmlns="urn:oasis:names:tc:xliff:document:1.2" version="1.2">
            <file source-language="en" target-language="en" datatype="plaintext" original="file.ext">
                <body>
                    <trans-unit id="seo.title">
                        <source>seo.title</source>
                        <target>%content_title% | Default title</target>
                    </trans-unit>
                    <trans-unit id="seo.description">
                        <source>seo.description</source>
                        <target>Default description. %content_description%</target>
                    </trans-unit>
                </body>
            </file>
        </xliff>

    .. code-block:: php

        // app/Resources/translations/messages.en.php
        return [
            'seo' => [
                'title'       => '%content_title% | Default title',
                'description' => 'Default description. %content_description',
            ],
        ];

    .. code-block:: yaml

        # app/Resources/translations/messages.en.yml
        seo:
            title:       "%content_title% | Default title"
            description: "Default description. %content_description%"

.. tip::

    You don't have to escape the percent characters here, since the
    Translation loaders do not try to evaluate container variables.

For changing the default translation domain, use the
``cmf_seo.translation_domain`` setting:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        cmf_seo:
            translation_domain: seo

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="utf-8"?>
        <container xmlns="http://symfony.com/schema/dic/services">
            <config xmlns="http://cmf.symfony.com/schema/dic/seo"
                translation-domain="seo"
            />
        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension(
            'cmf_seo' => [
                'translation_domain' => 'seo',
            ],
        );

.. _bundles-seo-alternate-locale:

Alternate Locales Support
-------------------------

Alternate locales are a way of telling search engines how to find translations
of the current page. The SeoBundle provides a way to manage alternate locales
and output them together with the other SEO information.

This feature is explained in :doc:`alternate_locale`.

Sitemap Support
---------------

The SEO bundle can help you provide XML sitemaps to be consumed by search engines.

This feature is explained in :doc:`sitemap`.

Conclusion
----------

That's it! You have now created a SEO optimized website using nothing more
than a couple of simple settings.

Now you can start reading the full :doc:`configuration reference
<configuration>` to learn even more about the settings.

.. _`SonataSeoBundle`: https://github.com/sonata-project/SonataSeoBundle
.. _`with composer`: https://getcomposer.org
.. _`packagist`: https://packagist.org/packages/symfony-cmf/seo-bundle
.. _`Sonata documentation`: https://sonata-project.org/bundles/seo/master/doc/index.html

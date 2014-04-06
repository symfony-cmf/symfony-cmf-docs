SeoBundle
=========

    This bundle provides a layer on top of the `SonataSeoBundle`_, to make it
    easier to collect SEO data from content documents.

Installation
------------

You can install this bundle `with composer`_ using the
``symfony-cmf/seo-content-bundle`` package on `Packagist`_.

Both the CmfSeoBundle and SonataSeoBundle must be registered in the
``AppKernel``::

    // app/appKernel.php

    // ...
    public function registerBundles()
    {
        $bundles = array(
            // ...
            new Sonata\SeoBundle\SonataSeoBundle(),
            new Symfony\Cmf\Bundle\SeoBundle\CmfSeoBundle(),
        );

        // ...

        return $bundles;
    }

Usage
~~~~~

The simplest use of this bundle would be to just set some configuration to the
``sonata_seo`` configuration section and use the twig helper in your templates.

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        sonata_seo:
            page:
                title: Page's default title
                metas:
                    names:
                        description: The default description of the page
                        keywords: default, sonata, seo

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('sonata_seo', array(
            'page' => array(
                'title' => 'Page's default title',
                'metas' => array(
                    'names' => array(
                        'description' => 'default description',
                        'keywords' => 'default, key, other',
                    ),
                ),
            ),
        ));
    
The only thing to do now is to use the twig helper in your templates:

.. code-block:: html+jinja

    <!-- app/Resources/views/base.html.twig -->
    <!DOCTYPE html>
    <html>
        <head>
            {{ sonata_seo_title() }}
            {{ sonata_seo_metadatas() }}
            {{ sonata_seo_link_canonical() }} // needed later
        </head>
        <body>
            <p>Some page body.</p>
        </body>
    </html>

This will render a page with the default title ("Page's default title") as
title element. The information definded for description and keywords will go
into the correct metatags.

.. seealso::

    To get a deeper look into the SonataSeoBundle, you should visit the
    `Sonata documentation`_.

Using SeoMetadata
-----------------

The basic example shown above works perfectly without the CmfSeoBundle. The
CmfSeoBundle provides more extension points to configure the SEO data with
data from the document (e.g. a ``StaticContent`` document). This is done by
using SEO metadata. This is SEO data which will be used for a particular
document. This metadata can hold:

* The title;
* The meta keywords;
* The meta description;
* The original URL (when more than one URL contains the same content).

This bundle provides two ways of using this metadata:

#. Implementing the ``SeoAwareInterface`` and persisting the ``SeoMetadata``
   with the object.
#. Using the extractors, to extract the ``SeoMetadata`` from already existing
   values (e.g. the title of the page).

You can also use both ways at the same time for the document. In that case,
the persisted ``SeoMetadata`` can be changed by the extractors.

Persisting the ``SeoMetadata`` with the document makes it easy to edit for the
admin, while using the extractors makes it perfect to use without doing
anything.

Both ways are documented in detail in seperate sections:

* :doc:`seo_aware`
* :doc:`extractors`

Choosing the Original Route Pattern
-----------------------------------

Search engines punish you, when you provide the same content under several
URLs. The CMF allows you to have several URLs for the same content if you
need that. There are two solutions to avoid penalties with search engines:

* Create a canonical link that identifies the original URL:
  ``<link rel="canonical" href="/route/org/content">``;
* Define an "original url" and redirect the other to that one.

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
        <config xmlns="http://cmf.symfony.com/schema/dic/seo"
            original-route-pattern="redirect"
        />

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension(
            'cmf_seo' => array(
                'original_route_pattern' => 'redirect',
            ),
        );

.. todo

Configuring the Default Values
------------------------------

You know now how to work with objects configuring SEO data. However, in some
cases the object doesn't provide any information. Then you have to configure a
default. You can configure the defaults for the SonataSeoBundle, these
defaults will be overriden by the metadata from the CmfSeoBundle. However, you
might want to combine.

Visiting the site with the url ``/seo-content`` (same template shown above)
will show a Page with "Documents own tile" as title, "This ist the text for
the description meta tag" in the description, "Seo, Content" in the keywords
and a canonical link with ``href="/original/url/of/content"``. But what about
some default string to just concatenate defaults and documents own values?
Just add some more configs to the cmf_seo configuration section.

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        sonata_seo:
            page:
                metas:
                    names:
                        keywords: default, sonata, seo
        cmf_seo:
            title: default_title_key
            description: default_title_key

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <container xmlns="http://symfony.com/schema/dic/services">
            <config
                    xmlns="http://cmf.symfony.com/schema/dic/seo"
                    title="default_title_key"
                    description="default_title_key">
            </config>
        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension(
            'sonata_seo', array(
                'page' => array(
                    'metas' => array(
                        'names' => array(
                            'keywords' => 'default, key, other',
                        ),
                    ),
                ),
            ),
            'cmf_seo' => array(
                'title'         => 'default_title_key',
                'description'   => 'default_description_key',
            ),
        );

As you will notice, you got the opportunity to set Symfony translation key for
your default values for title and description. So you will got
Multi-Language-Support out of the box. Just define your values for default
title/description as translations:

.. code-block:: xml

    <!-- app/Resources/translations/messages.en.xliff -->
    <?xml version="1.0" encoding="utf-8"?>
    <xliff xmlns="urn:oasis:names:tc:xliff:document:1.2" version="1.2">
        <file source-language="en" target-language="en" datatype="plaintext" original="messages.en.xliff">
            <body>
                <trans-unit id="default_title_key">
                    <source>default_title_key</source>
                    <target>%content_title% | Default title</target>
                </trans-unit>
                <trans-unit id="default_description_key">
                    <source>default_description_key</source>
                    <target>Default description. %content_description%</target>
                </trans-unit>
            </body>
        </file>
    </xliff>

If you want to concatenate your documents values with the default ones you
need them as parameters in you translation target.

.. tip::

    If you does not what to open a translation file for two entry, just set
    ``Default title | %%content_title%%``or ``Default description.
    %%content_description%%``.

For changing the default translation domain (messages), the SeoBundle provides
a configuration value:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        cmf_seo:
            translation_domain: AcmeDemoBundle

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <container xmlns="http://symfony.com/schema/dic/services">
            <config
                    xmlns="http://cmf.symfony.com/schema/dic/seo"
                    translation-domain="AcmeDemoBundle">
            </config>
        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension(
            'cmf_seo' => array(
                'translation_domain'         => 'AcmeDemoBundle',
            ),
        );

Preface
-------

Search engines punish you when you provide the same content under several
URLs. The CMF allows you to have several URLs for the same content if you
need that. There are two solutions to avoid penalties with search engines:

* Create a canonical link that identifies the original URL:
  ``<link rel="canonical" href="/route/org/content">``;
* Redirect to THE original url.

Both take care on search engines, which does not like it to have same content
under different routes.

The SeoBundle uses sonatas SeoBundle and its TwigHelper to render the the
`SeoMetadata` into your Pag. So you should have a look at the documentation at
`sonata seo documentation_`

For redirects instead of canonical links (default) set the following option:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        cmf_seo:
            original_route_pattern: redirect

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <container xmlns="http://symfony.com/schema/dic/services">
            <config
                    xmlns="http://cmf.symfony.com/schema/dic/seo"
                    original-route-pattern="redirect">
            </config>
        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension(
            'cmf_seo' => array(
                'original_route_pattern'    => 'redirect',
            ),
        );

This value will cause a redirect to the url persisted in the ``originalUrl``
property of the ``SeoMetadata``.

The SeoMetadata contains a form type for your Symfony Form. Just create you
form with the following key:

.. code-block:: php

    $formBuilder
        ...
        ->add('seoMetadata', 'seo_metadata', array('label' => false));
        ...
        ;

For SonataAdminBundle user the SeoBundle provides an admin extension to add
that form to your form configuration.


.. _`SonataSeoBundle`: https://github.com/sonata-project/SonataSeoBundle
.. _`with composer`: http://getcomposer.org
.. _`packagist`: https://packagist.org/packages/symfony-cmf/menu-bundle
.. _`Sonata documentation`: http://sonata-project.org/bundles/seo/master/doc/index.html

.. index::
    single: SeoContent; Bundles
    single: SeoContentBundle

SeoContentBundle
================

    This bundle provides a solution to make content bundles
    aware for Seach Engine Optimisation (SEO).

Preface
-------

As the Symfony CMF allows you to show one content on
multiple routes, it is a must have to avoid
duplicate content. There are two solutions to rid of it:

Create a canonical link with the reference to THE
original url:

    <link rel="canonical" href="/route/org/content">

or a redirect to the original url.

Both take care on search engines, which does not like
it to have same content under different routes.

The SonataSeoBundle still does a good job on handling
that stuff. But we need a thin wrapper around their
service to get the Symfony CMF content documents be
aware of those duplicate content problems. As sonatas
SeoBundle provides solutions for setting values for
meta tags and the page's title the SeoBundle will
contain "out-of-the-box" solution for the Symfony CMF
documents, too.

Installation
------------

You can install the bundle in 2 different ways:

* Use the official Git repository `with github`_
* Install it `with composer`_ (``symfony-Symfony CMF/seo-content-bundle`` on `Packagist`_).

As this bundle uses the SonataSeoBundle you should have a look of its `sonata seo documentation`_

Both bundles need to be registered in the ``appKernel``

    public function registerBundles()
    {
        $bundles = array(

            ...

            new \Sonata\SeoBundle\SonataSeoBundle(),
            new \Symfony\Cmf\Bundle\SeoBundle\CmfSeoBundle(),
        );

        ...

        return $bundles;
    }

Configuration
-------------

The first part of configuration is the one for the
sonata seo bundle. These settings are handled as
default values

.. configuration-block::

    .. code-block:: yaml

        sonata_seo:
            page:
                title: Default title
                metas:
                    names:
                        description: default description
                        keywords: default, key, other

    .. code-block:: php

        $container->loadFromExtension('sonata_seo', array(
            'page' => array(
                'title' => 'Default title',
                'metas' => array(
                    'names' => array(
                        'description' => 'default description',
                        'keywords' => 'default, key, other',
                    ),
                ),
            ),
        ));

Without any settings or work with the SeoBundle these settings
are enough to let the sonatas ``PageService`` know about your
defaults. These configs would cause a page title, a metatag for
description and one for the keywords, if you add the following
helpers to your html-head in your template:

    {{ sonata_seo_title() }}
    {{ sonata_seo_metadatas() }}

The SeoBundle adds some more options:

.. configuration-block::

    .. code-block:: yaml

        cmf_seo:
            title:
                strategy: append
                separator: ' | '
            content:
              strategy: canonical

    .. code-block:: php

        $container->loadFromExtension('cmf_seo', array(
            'title' => array(
                'strategy' => 'append',
                'separator'  => ' | '
                ),
            'content' => array(
                'stragegy' => 'canonical'
                )
        ));
    .. code-block:: xml
        <?xml version="1.0" charset="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services">

            <config xmlns="http://cmf.symfony.com/schema/dic/content">
                <cmf_seo>
                    <title
                        strategy="append"
                        separator=" | "
                    />
                    <content
                        strategy="canonical"
                    />
                </cmf_seo>
            </config>
        </container>

Now you are able to append or prepend a title to the default value.
Even replacing it by your title is possible. That means you should
set the ``cmf_seo.title.strategy`` either to `prepend` (default),
``append`` or ``replace``. Your values for description and keywords
will be appended to the sonatas default ones by a ". " or a ", ".
The ``cmf_seo.title.separator`` will configures the string separator
for appending or prepending the title.
In case of duplicate content you will need the ``cmf_seo.content.strategy``
The allowed values are ``canonical`` or ``redirect``. Firs will
cause a canonical link, the last forces a redirect to the original
url (set by the document's ``SeoMetadata``.

Usage
-----

The SeoContent bundle provides a ``SeoAwareInterface`` for content
documents, that needs to have some SEO properties. To persist
them and not to pollute the list of document properties, the
SeoContent bundle provides a ``SeoMetadate`` model. The
``SeoAwareInterface`` forces the content document to provide a
getter for that metadata. The bundle provides a
``SeoAwareContent`` document as an example. You can add it like
this:

    use Symfony\Cmf\Bundle\RoutingBundle\Doctrine\Phpcr\Route;
    use Symfony\Cmf\Bundle\SeoBundle\Doctrine\Phpcr\SeoAwareContent;
    use Symfony\Cmf\Bundle\SeoBundle\Model\SeoMetadata;

    // retrieve the route root node
    $routeRoot = $documentManager->find(null, '/cms/routes');

    //create the route for the document
    $route = new Route();
    $route->setPosition($routeRoot, 'seo-content');
    $route->setDefaults(
                array(
                    '_template' => 'Acme:Seo:index.html.twig'
                )
            );

    //retrieve the root document
    $rootDocument = $documentManager->find(null, '/cms/content');

    //create the seo aware document
    $seoDocument = new SeoAwareContent();
    $seoDocument->setParent($rootDocument);
    $seoDocument->setTitle('Same title as in original');
    $seoDocument->setName('seo-content');
    $seoDocument->setBody('Same Content as in Original');
    $seoDocument->addRoute($route);

    //set the seo metadata
    $seoMetadata = new SeoMetadata();
    //additional page title
    $seoMetadata->setTitle("Documents own tile");
    $seoMetadata->setMetaKeywords('Seo, Content');
    $seoMetadata->setMetaDescription(
        'This ist the text for the description meta tag'
    );
    $seoMetadata->setOriginalUrl("/original-url");
    $seoDocument->setSeoMetadata($seoMetadata);

    $manager->persist($seoDocument);
    $manager->flush();

The code adds a ``/seo-content`` route and a seo-content-document
``seo-content`` This means if you visit the route you will se the
content of the document. When you have a look at the title or the
meta tags, you will see something different. The title looks like
``Documents own tile | Default title`` and the meta tags contain the
description and keywords added by some default values.
(See in the Configuration).

From SeoMetadata to MetadataTag
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Every document that implements the ``SeoAwareInterface`` contain
some ``SeoMetadata``. Sonatas ``PageService`` works like a container
and is able to store these values for us. When adding one of sonatas
twig helpers

    {{ sonata_seo_title() }}
    {{ sonata_seo_metadatas() }}
    {{ sonata_seo_link_canonical() }}
    {{ sonata_seo_lang_alternates() }}

sonatas SeoBundle is able to create metatags from some values or setting
the content to the title tag. Without adding any ``SeoMetadata`` to
the document, sonatas SeoBundle would render the default values
served in their own config block. Symfony CMF's SeoBundle has a
Listener, which takes care on the documents. If there is a
content of type ``SeoAwareInterface`` a ``SeoPresentation``-Model
starts to work and fills sonatas PageService depending on the
strategies set in the SeoBundle's configuration block.


AdminExtension - FormType
~~~~~~~~~~~~

The ``SeoBundle`` provides an admin extension and a special form
type. If you are using SonataAdmin, you should add the extension
to the ``sonata_admin.extension`` like this:

    cmf_seo.content.admin_extension:
        implements:
          - Symfony\Cmf\Bundle\SeoBundle\Model\SeoAwareInterface

This will enable the extension and you will get a new tab with
a ``SeoMetadataType`` as a form type. This should help you on
updating the documents ``SeoMetadata``.

.. _`with composer`: http://getcomposer.org
.. _`symfony-cmf/menu-bundle`: https://packagist.org/packages/symfony-cmf/menu-bundle
.. _`with github`: git clone https://github.com/symfony-cmf/SeoContentBundle version path/to/
.. _`sonata seo documentation`: http://sonata-project.org/bundles/seo/master/doc/index.html
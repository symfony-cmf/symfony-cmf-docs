.. index::
    single: SeoContent; Bundles
    single: SeoContentBundle

SeoContentBundle
================

    This bundle provides a soluton to make content bundles
    aware of SEO.

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

* Use the official Git repository (https://github.com/symfony-Symfony CMF/SeoContentBundle);
* Install it via Composer (``symfony-Symfony CMF/seo-content-bundle`` on `Packagist`_).

Configuration
-------------

The first part of configuration is the one for the
sonata seo bundle. These settings are handled as
default values


    sonata_seo:
      page:
        title: Default title
        metas:
          names:
              description: default description
              keywords: default, key, other

Without any settings or work with the SeoBundle these settings
are enough to let the sonatas ``PageService`` know about your
default. When you add the twig helpers to your template you
will get the values in your title or your meta tags.

The SeoBundle adds some more options:

Symfony CMF_seo:
    title:
        strategy: append
        bond_by: ' | '
    content:
      strategy: canonical

Now you are able to decide if a title (set in a SeoAwareContent)
is appending, prepending (default) the default title or replacing
it. The strategy values can be ``prepend``, ``appen`` or
``replace``. You are also able to set a string, which bond the
default and the contents own title property by setting a value
to ``Symfony CMF_seo.title.bond_by``. In case of duplicate
content it is the task of the developer to set the strategy either
to ``canonical`` (a canonical link will be created) or to
``redirect`` (default).

Usage
-----

The SeoContent bundle provides a ``SeoAwareInterface`` for content
documents, that needs to have some SEO properties. To persist
them and not to pollute the list of document properties, the
SeoContent bundle provides a ``SeoMetadate`` model. The
``SeoAwareInterface`` forces the content document to provide a
getter for that metadata. The bundle provides a
``SeoAwareContent`` document as an example. You can add it like
this::

    use Symfony\Cmf\Bundle\RoutingBundle\Doctrine\Phpcr\Route;
    use Symfony\Cmf\Bundle\SeoBundle\Doctrine\Phpcr\SeoAwareContent;
    use Symfony\Cmf\Bundle\SeoBundle\Model\SeoMetadata;

    // retrieve the route root node
    $routeRoot = $documentManager->find(null, '/cms/routes');

    //create the route for the document
    $route = new Route();
    $route->setPosition($routeRoot, 'hello');
    $routeset->Defaults(
                array(
                    '_controller'=> 'cmf_seo.controller:indexAction',
                    RouteObjectInterface::TEMPLATE_NAME => 'Acme:Seo:index.html.twig'
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
    $seoMetadata->setTitle("Special Title");
    $seoMetadata->setMetaKeywords('Seo, Content');
    $seoMetadata->setMetaDescription(
        'This ist the text for the description meta tag'
    );
    $seoMetadata->setOriginalUrl("/org-ur");
    $seoDocument->setSeoMetadata($seoMetadata);

    $manager->persist($seoDocument);
    $manager->flush();


The code adds a ``/seo-content`` route and a seo-content-document
``seo-content`` This means if you visit the route you will se the
content of the document. When you have a look at the title or the
meta tags, you will see something different. The title looks like
``Special Title | Default`` and the meta tags contain the
description and keywords added by some default values.
(See in the Configuration).

From SeoMetadata to MetadataTag
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


As the the content document provides the ``SeoAwareInterface`` it
should be able to return the ``SeoMetadata``. But how does these
properties get into your page?

At the moment you need to add the following lines to your
controller:

    //when there are some seo meta data they will be handled by a special service
    if ($contentDocument instanceof SeoAwareInterface) {
        $this->seoPage->setSeoMetadata($contentDocument->getSeoMetadata());
        $this->seoPage->setMetadataValues();

        //have a look if the strategy is redirect and if there is a route to redirect to
        if ($url = $this->seoPage->getRedirect()) {
            print("should be redirected to $url");
            exit;
        }
    }

and implement the ``SeoAwareContentControllerInterface`` to
get the ``SeoPresentation``-Model which is responsible to
set the properties of the SeoMetadata to Sonatas
``PageService``. Sonata does not only provide its service,
it delivers a bunch of twig helpers, which are able to put
the seo metadata into your page. Have a look at these examples:

    {% extends "::base.html.twig" %}

    {% block seo_head %}
        {{ sonata_seo_title() }}
        {{ sonata_seo_metadatas() }}
        {{ sonata_seo_link_canonical() }}
        {{ sonata_seo_lang_alternates() }}
    {% endblock %}



The FormType
~~~~~~~~~~~~

To set all these metadata we provide a FormTye too. The
``SeoMetadataType`` contains all the fields you would
need for the ``SeoMetadata`` an example for the
SonataAdmin would look like this:

    ->with('form.group_seo')
        ->add('seoMetadata', 'seo_metadata', array('label'=>false))
    ->end()



.. index::
    single: Resource; Repository

Resource Repositories
=====================

Repositories are the access point of the ResourceBundle, allowing you to find
and move resources. In the bundle, currently two repository types are provided:
Doctrine PHPCR-ODM and PHPCR.

Creating a new repository is as simple as configuring the ResourceBundle:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        cmf_resource:
            repositories:
                # defines a repository named "default" using a Doctrine PHPCR-ODM backend
                default:
                    type: doctrine/phpcr-odm

                # defines an "other_site" repository using a PHPCR backend with
                # /cms/other-site.com as root
                other_site:
                    type:     phpcr/phpcr
                    basepath: /cms/other-site.com

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services">

            <config xmlns="http://cmf.symfony.com/schema/dic/resource">
                <!-- defines a repository named "default" using a Doctrine PHPCR-ODM backend -->
                <repository name="default"
                    type="doctrine/phpcr-odm"
                />

                <!-- defines an "other_site" repository using a PHPCR backend
                     with /cms/other-site.com as root -->
                <repository name="other_site"
                    type="phpcr/phpcr"
                    basepath="/cms/other-site.com"
                />
            </config>
        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('cmf_resource', [
            'repositories' => [
                // defines a repository named "default" using a Doctrine PHPCR-ODM backend
                'default' => [
                    'type' => 'doctrine/phpcr-odm',
                ],

                // defines an "other_site" repository using a PHPCR backend with
                // /cms/other-site.com as root
                'other_site' => [
                    'type'     => 'phpcr/phpcr',
                    'basepath' => '/cms/other-site.com',
                ],
            ],
        ]);

Both repositories allow to configure a ``basepath``. This becomes the root of
the repository.

If you have configured your repositories, you can start using them as
services::

    namespace AppBundle\Controller;

    use Symfony\Bundle\FrameworkBundle\Controller\Controller;

    class HomepageController extends Controller
    {
        public function indexAction()
        {
            // The generated service ID is cmf_resource.repository.<REPOSITORY NAME>
            $defaultRepository = $this->get('cmf_resource.repository.default');

            // Get resources directly using get($path)
            $homepageResource = $defaultRepository->get('/pages/homepage');

            // Or find resources using a glob pattern
            $menuResources = $defaultRepository->get('/menu/*-item');

            // Get the CMF documented related to this resource
            $homepageDocument = $homepageResource->getPayload();

            return $this->render('static/page.html.twig', [
                'page' => $homepageDocument
            ]);
        }
    }

Besides retrieving and finding documents, repositories also provide some
methods to edit resources:

``remove($path)``
    Remove a resource at the given path (i.e. ``remove('/cms/pages/homepage')``)
    or multiple documents using a glob pattern (i.e. ``/cms/menu/legacy-*``).

``move($path, $targetPath)``
    Move a resource (i.e. ``move('/cms/pages/contact', '/cms/pages/about-us/contact')``)
    or multiple resources (i.e.  ``move('/cms/menu/contact-*', '/cms/menu/about-us')``).

``reorder($path, $position)``
    Reorders a resource relative to it's siblings. For instance, use position
    ``0`` to set it as first child, ``2`` to set it as third child and a high
    number to set it as last child.

.. index::
    single: Resource; Description Enhancer

Resource Description Enhancers
==============================

The resources retrieved from the :doc:`resource repositories <repositories>`
can be enhanced with descriptions: Bits of information about the resource. This
is done by so-called *description enhancers*.

Configuring Description Enhancers
---------------------------------

In order to use a description enhancers, enable it in your configuration:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        cmf_resource:
            description:
                # enables two enhancers
                enhancers: [doctrine_phpcr_odm, your_custom_enhancer]

            repositories:
                # ...

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services">

            <config xmlns="http://cmf.symfony.com/schema/dic/resource">
                <description>
                    <!-- enables two enhancers -->
                    <enhancer>doctrine_phpcr_odm</enhancer>
                    <enhancer>your_custom_enhancer</enhancer>
                </description>

                <!-- ... -->
            </config>
        </container>

    .. code-block:: php

        // app/config/config.yml
        $container->loadFromExtension('cmf_resource', [
            'description' => [
                // enables two enhancers
                'enhancers' => ['doctrine_phpcr_odm', 'your_custom_enhancer'],
            ],

            'repositories' => [
                // ...
            ],
        ]);

Retrieving the Resource Description
-----------------------------------

The description for a specific resource can be retrieved using the
``cmf_resource.description.factory`` service::

    namespace AppBundle\Controller;

    use Symfony\Cmf\Component\Resource\Description\Descriptor;

    class PageController extends Controller
    {
        public function indexAction()
        {
            $homepageResource = $this->get('cmf_resource.repository.default')->get('/pages/homepage');

            $descriptionFactory = $this->get('cmf_resource.description.factory');
            $resourceDescription = $descriptionFactory->getPayloadDescriptionFor($homepageResource);

            // check if there is a title descriptor
            if ($resourceDescription->has(Descriptor::TITLE)) {
                // get a descriptor (i.e. the title)
                $title = $resourceDescription->get(Descriptor::TITLE);
            }

            // get all descriptors
            $descriptors = $resourceDescription->all();

            // ...
        }
    }

Descriptors can contain any type and consist of an identifier and the value.
Some common identifiers are defined in the ``Descriptor`` class, but any
descriptor identifier is allowed.

CMF Description Enhancers
-------------------------

Some CMF bundles ship description enhancers to add specific descriptors used by that bundle:

:doc:`../tree_browser/introduction`
    Ships a ``cmf_tree_icons`` enhancer, which sets an ``icon`` description to
    an icon used in the tree.

:doc:`../sonata_phpcr_admin_integration/introduction`
    Ships a ``sonata_phpcr_admin`` enhancer, which sets edit links to the admin
    dashboard and payload title and type aliases using the related Admin class.

:doc:`introduction`
    Ships a ``doctrine_phpcr_odm`` enhancer, which sets allowed children classes.

:doc:`introduction`
    Ships a ``sylius_resource`` enhancer, adding CRUD links for the SyliusResourceBundle_.

Creating a Custom Enhancer
--------------------------

You can create your own enhancer by implementing ``DescriptionEnhancerInterface``::

    // src/AppBundle/Description/PageEnhancer.php
    namespace AppBundle\Description;

    use AppBundle\Document\Page;
    use Symfony\Cmf\Component\Resource\Description\Descriptor;
    use Symfony\Cmf\Component\Resource\Description\Description;
    use Symfony\Cmf\Component\Resource\Description\DescriptionEnhancerInterface;
    use Symfony\Cmf\Component\Resource\Puli\Api\PuliResource;

    class PageEnhancer implements DescriptionEnhancerInterface
    {
        public function supports(PuliResource $resource)
        {
            // check if the resource is supported by this enhancer (i.e. whether it's an app page).
            return $resource->getPayload() instanceof Page;
        }

        public function enhance(Description $description)
        {
            $resource = $description->getResource();

            // set the payload title descriptor to ``Page#getTitle()``
            $description->set(Descriptor::PAYLOAD_TITLE, $resource->getTitle());
        }
    }

Then, create a service and tag it with ``cmf_resource.description.enhancer``:

.. configuration-block::

    .. code-block:: yaml

        # app/config/services.yml
        services:
            app.page_enhancer:
                class: AppBundle\Description\PageEnhancer
                tags:
                    - { name: cmf_resource.description.enhancer, alias: app_page }

    .. code-block:: xml

        <!-- app/config/services.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services">

            <services>
                <service id="app.page_enhancer" class="AppBundle\Description\PageEnhancer">
                    <tag name="cmf_resource.description.enhancer" alias="app_page" />
                </service>
            </services>
        </container>

    .. code-block:: php

        // app/config/services.php
        use AppBundle\Description\PageEnhancer;

        $container->register('app.page_enhancer', PageEnhancer::class)
            ->addTag('cmf_resource.description.enhancer', [
                'alias' => 'app_page',
            ])
        ;

After this, you can enable your enhancer using it's alias (``app_page``).

.._SyliusResourceBundle: http://docs.sylius.org/en/latest/bundles/SyliusResourceBundle

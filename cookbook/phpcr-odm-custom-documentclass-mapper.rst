Using a custom document class mapper with PHPCR-ODM
===================================================

The default document class mapper of PHPCR-ODM uses the attribute ``phpcr:class``
to store and retrieve the document class of a node. When accessing an existing
PHPCR repository, you might need different logic to decide on the class.

You can extend the ``DocumentClassMapper`` or implement ``DocumentClassMapperInterface``
from scratch. The important methods are getClassName that needs to find the class name
and writeMetadata that needs to make sure the class of a newly stored document can be
determined when loading it again.

Then you can overwrite the doctrine.odm_configuration service to call setDocumentClassMapper
on it. An example from the `symfony cmf sandbox`_ (magnolia_integration branch):

.. configuration-block::

    .. code-block:: yaml

        # Resources/config/services.yml

        # if you want to overwrite default configuration, otherwise use a
        # custom name and specify in odm configuration block

        doctrine.odm_configuration:
            class: %doctrine_phpcr.odm.configuration.class%
            calls:
                - [ setDocumentClassMapper, [@sandbox_magnolia.odm_mapper] ]

        sandbox_magnolia.odm_mapper:
            class: "Sandbox\MagnoliaBundle\Document\MagnoliaDocumentClassMapper"
            arguments:
                - 'standard-templating-kit:pages/stkSection': 'Sandbox\MagnoliaBundle\Document\Section'

Here we create a mapper that uses a configuration to read node information and
map that onto a document class.

If you have several repositories, you can use one configuration per repository.
See :ref:`bundle-phpcr-odm-multiple-phpcr-sessions`.

.. _`symfony cmf sandbox`: https://github.com/symfony-cmf/cmf-sandbox/tree/magnolia_integration
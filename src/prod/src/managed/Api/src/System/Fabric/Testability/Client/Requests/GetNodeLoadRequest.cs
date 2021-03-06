// ------------------------------------------------------------
// Copyright (c) Microsoft Corporation.  All rights reserved.
// Licensed under the MIT License (MIT). See License.txt in the repo root for license information.
// ------------------------------------------------------------

// CS1591 - Missing XML comment for publicly visible type or member 'Type_or_Member' is disabled in this file because it does not ship anymore.
#pragma warning disable 1591

namespace System.Fabric.Testability.Client.Requests
{
    using System;
    using System.Globalization;
    using System.Threading;
    using System.Threading.Tasks;
    using System.Fabric.Testability.Client.Structures;

    public class GetNodeLoadRequest : FabricRequest
    {
        public GetNodeLoadRequest(IFabricClient fabricClient, string nodeName, TimeSpan timeout)
            : base(fabricClient, timeout)
        {
            this.NodeName = nodeName;
        }

        public string NodeName
        {
            get;
            private set;
        }

        public override string ToString()
        {
            return string.Format(CultureInfo.InvariantCulture, "GetNodeLoad of Node {0} with timeout {1}", this.NodeName, this.Timeout);
        }

        public override async Task PerformCoreAsync(CancellationToken cancellationToken)
        {
            this.OperationResult = await this.FabricClient.GetNodeLoadAsync(this.NodeName, this.Timeout, cancellationToken);
        }
    }
}


#pragma warning restore 1591
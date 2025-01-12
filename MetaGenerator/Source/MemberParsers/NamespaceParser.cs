using PCRE;
using System.Collections.Generic;

namespace MetaGenerator
{
    public class NamespaceInfo
    {
        public readonly string Name;
        private readonly int _startPosition;
        private readonly int _endPosition;

        public NamespaceInfo(string name, int startPosition, int endPosition)
        {
            Name = name;
            _startPosition = startPosition;
            _endPosition = endPosition;
        }

        public bool IsBound(int position) => position > _startPosition && position < _endPosition;
    }

    public class NamespaceParser : PrimaryMemberParser
    {
        private readonly List<NamespaceInfo> _namespaceInfos = [];

        public NamespaceParser(HeaderParser headerParser) : base(headerParser)
        {
        }

        public NamespaceInfo[] Parse()
        {
            Parse(Text);
            return _namespaceInfos.ToArray();
        }

        private void Parse(string text, int outerNamespaceOffset = 0)
        {
            IEnumerable<PcreMatch> matches = FindMatches(@"namespace\s+(\w+)\s*(?<body>\{(?:[^{}]|(?&body))*\})", text);
            ParseInfos(matches, outerNamespaceOffset);
        }

        private void ParseInfos(IEnumerable<PcreMatch> matches, int outerNamespaceOffset)
        {
            foreach (var match in matches)
            {
                int originOffset = match.Groups[2].Index + outerNamespaceOffset;

                var namespaceInfo = new NamespaceInfo(
                    match.Groups[1].Value,
                    originOffset,
                    match.Groups[2].EndIndex + outerNamespaceOffset);

                _namespaceInfos.Add(namespaceInfo);

                Parse(match.Groups[2].Value, originOffset);
            }
        }
    }
}

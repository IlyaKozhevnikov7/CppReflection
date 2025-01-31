using PCRE;
using System.Collections.Generic;
using System.Linq;

namespace MetaGenerator
{
    public class ConstructorParser : SecondaryMemberParser
    {
        public ConstructorParser(string text) : base(text)
        {
        }

        public ConstructorInfo[] Parse(string className)
        {
            var matches = FindMatches($@"CONSTRUCTOR(?<attr>\((?:[^()]|(?&attr))*\))\s*[\S\s]*?{className}\s*(?<args>\((?:[^()]|(?&args))*\))", Text);
            return ParseInfos(matches);
        }

        private ConstructorInfo[] ParseInfos(IEnumerable<PcreMatch> matches)
        {
            var infos = MemberInfoBase.Construct<ConstructorInfo>(matches.Count());
                
            int i = 0;
            foreach (var match in matches)
            {        
                infos[i].parameters = match[2];
                infos[i].attributeInfos = ParseAttributes(match.Groups["attr"].Value);
                ++i;
            }
        
            return infos;
        }
    }
}

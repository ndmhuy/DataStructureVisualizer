#ifndef IPAYLOADVISITOR_H
#define IPAYLOADVISITOR_H

// Forward declarations
struct LinkedListPayload;
struct TreePayload;
struct HeapPayload;
struct GraphPayload;
struct SingleSourcePayload;
struct AStarPayload;
struct AllPairsPayload;
struct GridPayload;
struct MenuAnimPayload;
struct DecorationPayload;
struct TopBarPayload;
class IPayloadVisitor {
public:
    virtual ~IPayloadVisitor() = default;
    
    virtual void visit(const LinkedListPayload& payload) = 0;
    virtual void visit(const TreePayload& payload) = 0;
    virtual void visit(const HeapPayload& payload) = 0;
    virtual void visit(const GraphPayload& payload) = 0;
    virtual void visit(const SingleSourcePayload& payload) = 0;
    virtual void visit(const AStarPayload& payload) = 0;
    virtual void visit(const AllPairsPayload& payload) = 0;
    virtual void visit(const GridPayload& payload) = 0;
    virtual void visit(const MenuAnimPayload& payload) = 0;
    virtual void visit(const DecorationPayload& payload) = 0;
    virtual void visit(const TopBarPayload& payload) = 0;
};

#endif // IPAYLOADVISITOR_H
